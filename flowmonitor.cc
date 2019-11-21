#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (2));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor=flowmon.InstallAll();

  NS_LOG_INFO("Run Simulation.");
  Simulator::Stop(Seconds(11.0));
  Simulator::Run ();

  monitor->CheckForLostPackets();

  Ptr<Ipv4FlowClassifier> classifier=DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
  std::map<FlowId,FlowMonitor::FlowStats> stats=monitor->GetFlowStats();
  for(std::map<FlowId,FlowMonitor::FlowStats>::const_iterator i=stats.begin();i!=stats.end();++i)
    {
        Ipv4FlowClassifier::FiveTuple t=classifier->FindFlow(i->first);
        std::cout<<"Flow: "<<i->first<<"\n"<<"Source Address: "<<t.sourceAddress<<"\n"
        <<"Destination Address: "<<t.destinationAddress<<"\n"<<"Source Port: "<<t.sourcePort
        <<"\n"<<"Destination port: "<<t.destinationPort<<"\n";

        std::cout<<"Flow: "<<i->first<<"("<<t.sourceAddress<<"->"<<t.destinationAddress<<")\n";
        std::cout<<"Tx Bytes: "<<i->second.txBytes<<"\n";
        std::cout<<"Rx Bytes: "<<i->second.rxBytes<<"\n";
        std::cout<<"Tx Packets: "<<i->second.txPackets<<"\n";
        std::cout<<"Rx Packets: "<<i->second.rxPackets<<"\n";
        std::cout<<"Total time taken for the transmission: "
        <<i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()<<"\n";
        std::cout<<"Throughput: "<<i->second.rxBytes*8.0/
        (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds())/1000/1000<<"Mbps\n\n";

    }


  Simulator::Destroy ();
  return 0;
}
/*

bmsce@bmsce-Precision-T1700:~$ cd /home/bmsce/ns-allinone-3.29/ns-3.29
bmsce@bmsce-Precision-T1700:~/ns-allinone-3.29/ns-3.29$ ./waf
Waf: Entering directory `/home/bmsce/ns-allinone-3.29/ns-3.29/build'
[1904/1957] Compiling scratch/flowmonitor.cc
[1905/1957] Compiling scratch/netanim22.cc
[1916/1957] Linking build/scratch/flowmonitor
[1921/1957] Linking build/scratch/netanim22
Waf: Leaving directory `/home/bmsce/ns-allinone-3.29/ns-3.29/build'
Build commands will be stored in build/compile_commands.json
'build' finished successfully (4.664s)

Modules built:
antenna                   aodv                      applications              
bridge                    buildings                 config-store              
core                      csma                      csma-layout               
dsdv                      dsr                       energy                    
fd-net-device             flow-monitor              internet                  
internet-apps             lr-wpan                   lte                       
mesh                      mobility                  mpi                       
netanim (no Python)       network                   nix-vector-routing        
olsr                      point-to-point            point-to-point-layout     
propagation               sixlowpan                 spectrum                  
stats                     tap-bridge                test (no Python)          
topology-read             traffic-control           uan                       
virtual-net-device        wave                      wifi                      
wimax                     

Modules not built (see ns-3 tutorial for explanation):
brite                     click                     openflow                  
visualizer                

bmsce@bmsce-Precision-T1700:~/ns-allinone-3.29/ns-3.29$ ./waf  --run scratch/flowmonitor
Waf: Entering directory `/home/bmsce/ns-allinone-3.29/ns-3.29/build'
Waf: Leaving directory `/home/bmsce/ns-allinone-3.29/ns-3.29/build'
Build commands will be stored in build/compile_commands.json
'build' finished successfully (6.712s)
At time 2s client sent 512 bytes to 10.1.1.2 port 9
At time 2.00287s server received 512 bytes from 10.1.1.1 port 49153
At time 2.00287s server sent 512 bytes to 10.1.1.1 port 49153
At time 2.00573s client received 512 bytes from 10.1.1.2 port 9
At time 3s client sent 512 bytes to 10.1.1.2 port 9
At time 3.00287s server received 512 bytes from 10.1.1.1 port 49153
At time 3.00287s server sent 512 bytes to 10.1.1.1 port 49153
At time 3.00573s client received 512 bytes from 10.1.1.2 port 9
Flow: 1
Source Address: 10.1.1.1
Destination Address: 10.1.1.2
Source Port: 49153
Destination port: 9
Flow: 1(10.1.1.1->10.1.1.2)
Tx Bytes: 1080
Rx Bytes: 1080
Tx Packets: 2
Rx Packets: 2
Total time taken for the transmission: 1.00287
Throughput: 0.0086153Mbps

Flow: 2
Source Address: 10.1.1.2
Destination Address: 10.1.1.1
Source Port: 9
Destination port: 49153
Flow: 2(10.1.1.2->10.1.1.1)
Tx Bytes: 1080
Rx Bytes: 1080
Tx Packets: 2
Rx Packets: 2
Total time taken for the transmission: 1.00287
Throughput: 0.0086153Mbps
*/
