#include "server.hpp"
#include "tree.hpp"
#include <algorithm>
#include <csignal>
#include <iostream>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>
#include <zmq.hpp>

// ZMQ_SNDTIMEO: Maximum time before a send operation returns with EAGAIN
// ZMQ_LINGER: linger period determines how long pending messages which have yet to be sent to a peer shall linger in memory after a socket is closed

using namespace std;

int main()
{

    zmq::context_t context(1);
    zmq::socket_t mainSocket(context, ZMQ_REQ);

    mainSocket.setsockopt(ZMQ_SNDTIMEO, 2000);
    int linger = 0;
    mainSocket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    int port = bindSocket(mainSocket);

    Tree tree;

    int childPid = 0;
    int childId = 0;
    int createNodeId;

    int id;
    char excmd;
    string word;
    int val;

    string sendingMsg;
    string receivedMsg;

    string cmd;
    while (cout << "> " && cin >> cmd) {
        if (cmd == "create") {
            cin >> createNodeId;
            if (childPid == 0) {
                childPid = fork();
                if (childPid == -1) {
                    cout << "Error: fork fails\n";
                    childPid = 0;
                    exit(1);
                }
                else if (childPid == 0) {
                    createNode(createNodeId, port);
                }
                else {
                    childId = createNodeId;
                    sendMessage(mainSocket, "pid");
                    receivedMsg = receiveMessage(mainSocket);
                }
            }
            else {
                ostringstream sendingMsgStream;
                sendingMsgStream << "create " << createNodeId;
                sendMessage(mainSocket, sendingMsgStream.str());
                receivedMsg = receiveMessage(mainSocket);
            }

            if (receivedMsg.substr(0, 2) == "OK") {
                tree.insert(createNodeId);
            }

            cout << receivedMsg << "\n";
        }
        else if (cmd == "remove") {
            if (childPid == 0) {
                cout << "Error: Not found\n";
                continue;
            }
            cin >> createNodeId;
            if (createNodeId == childId) {
                kill(childPid, SIGTERM);
                kill(childPid, SIGKILL);
                childId = childPid = 0;
                cout << "OK\n";
                tree.erase(createNodeId);
                continue;
            }
            sendingMsg = "remove " + to_string(createNodeId);
            sendMessage(mainSocket, sendingMsg);
            receivedMsg = receiveMessage(mainSocket);
            if (receivedMsg.substr(0, 2) == "OK")
                tree.erase(createNodeId);
            cout << receivedMsg << "\n";
        }
        else if (cmd == "exec") {
            cin >> id >> excmd >> word;
            if (excmd == '+')
                cin >> val;

            sendingMsg = "exec " + to_string(id);
            sendMessage(mainSocket, sendingMsg);

            receivedMsg = receiveMessage(mainSocket);
            if (receivedMsg == "Node is available") {
                if (excmd == '+') {
                    tree.dictInsertWord(id, word, val);
                    cout << "OK:" << id << endl;
                }
                if (excmd == '?') {
                    cout << "OK:" << id << ": ";
                    tree.dictGetWord(id, word);
                }
            }
            else {
                cout << receivedMsg << endl;
            }
        }
        else if (cmd == "ping") {
            cin >> id;
            vector<int> nodesList = tree.getNodesList();
            bool nodeExists = binary_search(nodesList.begin(), nodesList.end(), id);
            if (nodeExists == 0) {
                cout << "Error: Not found\n";
            }
            else {
                sendMessage(mainSocket, "exec " + to_string(id));
                receivedMsg = receiveMessage(mainSocket);
                istringstream is;
                if (receivedMsg.substr(0, 5) == "Error")
                    cout << "OK:0\n";
                else
                    cout << "OK:1\n";
            }
        }
        else if (cmd == "pingall") {
            vector<int> nodesList = tree.getNodesList();
            if (nodesList.empty()) {
                cout << "Error: Tree is empty\n";
                continue;
            }

            sendMessage(mainSocket, "pingall");
            receivedMsg = receiveMessage(mainSocket);
            istringstream is;
            if (receivedMsg.substr(0, 5) == "Error")
                is = istringstream("");
            else
                is = istringstream(receivedMsg);

            set<int> receivedNodes;
            int rec_id;
            while (is >> rec_id) {
                receivedNodes.insert(rec_id);
            }

            cout << "Received nodes: ";
            for (const int &i : receivedNodes)
                cout << i << " ";

            cout << "\nNodes list:     ";
            for (const int &i : nodesList)
                cout << i << " ";

            cout << "\n";
        }
        else if (cmd == "exit") {
            break;
        }
        else {
            cout << "Unknown command\n";
        }
    }
    return 0;
}
