

#include "firewall/DPIServer.h"
#include "firewall/ArgumentParser.h"

#include <iostream>
#include <getopt.h>

constexpr std::uint16_t DEFAULT_RX_AMOUNT = 1;
constexpr std::uint16_t DEFAULT_TX_AMOUNT = 1;
constexpr std::uint16_t DEFAULT_MAX_BURST_SIZE = 64;

void PrintUsage() {
    std::cout << "Usage:" << std::endl << std::endl;
    std::cout << pcpp::AppName::get() << "  -d <dpdk_port> [-r <rx_queue_amount>] [-t <tx_queue_amount>]" << std::endl
              << std::endl;
    std::cout << "Options:" << std::endl << std::endl;
    std::cout << "  -d --dpdk-port <dpdk_port>          : DPDK Port ID" << std::endl;
    std::cout << "  -r --rx-queues <rx_queue_amount>    : Amount of RX queues to be opened" << std::endl;
    std::cout << "  -t --tx-queues <tx_queue_amount>    : Amount of TX queues to be opened" << std::endl;
    std::cout << "  -h --help                           : Displays this help message" << std::endl;
}

void ParseArgs(int argc, char *argv[], DPIServer::DPIServerArgs &args) {

    args.m_RxQueues = DEFAULT_RX_AMOUNT;
    args.m_TxQueues = DEFAULT_TX_AMOUNT;
    args.m_MaxBurstSize = DEFAULT_MAX_BURST_SIZE;

    ArgumentParser parser(argc, argv);

    if (!parser.OptionExists("-d")) {
        std::cerr << "DPDK port flag is mandatory (-d <dpdk_port>)" << std::endl;
        std::exit(1);
    }

    std::string port = parser.GetOption("-d");
    args.m_DpdkPortId = std::stoi(port);

    if (parser.OptionExists("-h")) {
        PrintUsage();
        std::exit(0);
    }

    if (parser.OptionExists("-b"))
        args.m_MaxBurstSize = std::stoul(parser.GetOption("-b")) & 0xFFFF;

    if (parser.OptionExists("-r"))
        args.m_RxQueues = std::stoul(parser.GetOption("-r")) & 0xFFFF;

    if (parser.OptionExists("-t"))
        args.m_TxQueues = std::stoul(parser.GetOption("-t")) & 0xFFFF;

//    struct option options[] = {
//            {"dpdk-port", required_argument, nullptr, 'd'},
//            {"burst-size", optional_argument, nullptr, 'b'},
//            {"rx-queues", optional_argument, nullptr, 'r'},
//            {"tx-queues", optional_argument, nullptr, 't'},
//            {"help", no_argument, nullptr, 'h'}
//    };
//
//    int optionIndex = 0;
//    int opt;
//    while ((opt = getopt_long(argc, argv, "d:r:t", options, &optionIndex)) != -1) {
//        switch (opt) {
//            case 0:
//                break;
//            case 'd':
//                args.m_DpdkPortId = std::strtoul(optarg, nullptr, 10);
//                break;
//            case 'r':
//                args.m_TxQueues = std::strtoul(optarg, nullptr, 10) & 0xFFFF;
//                break;
//            case 't':
//                args.m_RxQueues = std::strtoul(optarg, nullptr, 10) & 0xFFFF;
//                break;
//            case 'h':
//                PrintUsage();
//                std::exit(0);
//            default:
//                PrintUsage();
//                std::cerr << "Unknown option flag <" << static_cast<char>(opt) << '>' << std::endl;
//                std::exit(1);
//        }
//    }


}

int main(int argc, char *argv[]) {
    try {
        std::cout << "Initializing DPDK..." << std::endl;
        bool dpdkInitialized = pcpp::DpdkDeviceList::initDpdk(pcpp::getCoreMaskForAllMachineCores(), 16 * 1024 - 1);
        if (!dpdkInitialized) {
            throw DPIServer::DpdkInitializationError();
        }
        std::cout << "Successfully initialized DPDK!" << std::endl;

        DPIServer::DPIServerArgs args;
        ParseArgs(argc, argv, args);
        args.m_ServerPortNumber = 5000;
        args.m_DhcpEnabled = false;
        args.m_ServerIpAddress = "10.100.102.30";

        DPIServer server(args);

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

    catch (const DPIServer::DpdkInitializationError &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const DPIServer::UnknownDeviceException &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const DPIServer::MultiQueueException &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const DPIServer::InvalidIpAddressException &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "UNHANDLED EXCEPTION CAUGHT: " << e.what() << std::endl;
    }
}