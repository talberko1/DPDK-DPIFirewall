

#include "firewall/logic/DpdkServer.h"
#include "firewall/DPIServer.h"

#include <iostream>


int main(int argc, char *argv[]) {
    try {

        DPIServer::DPIServerArgs args;
        args.m_DpdkPortId = 0;
        args.m_RxQueues = 1;
        args.m_TxQueues = 1;
        args.m_ServerPortNumber = 5000;
        args.m_DhcpEnabled = false;
        args.m_ServerIpAddress = "10.100.102.30";
        args.m_MaxBurstSize = 64;
        args.m_MBufPoolSize = 16 * 1024 - 1;

        DPIServer server(64, args);

        bool running = true;

        while (running) {
            std::cout << "Firewall# ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "start")
                server.Start();
            if (input == "close") {
                if (server.Running())
                    server.Close();
            }
            if (input == "exit") {
                if (server.Running())
                    server.Close();
                running = false;
            }
        }
    }

    catch(const DpdkServer::DpdkInitializationError &e) {
        std::cout << e.what() << std::endl;
    }
    catch (const DpdkServer::UnknownDeviceException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (const DpdkServer::MultiQueueException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (const DPIServer::InvalidIpAddressException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "UNHANDLED EXCEPTION CAUGHT: " << e.what() << std::endl;
    }
}