//
// Copyright (C) 2010 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//


#ifndef __TCP_LWIP_CONNECTION_H
#define __TCP_LWIP_CONNECTION_H

#include "INETDefs.h"

#include "Address.h"
#include "lwip/tcp.h"
#include "TCPSegment.h"
#include "TCPCommand_m.h"

// forward declarations:
class TCP_lwIP;
class TCPConnectInfo;
class TcpLwipReceiveQueue;
class TcpLwipSendQueue;
class INetStack;
class INetStreamSocket;
class TCPStatusInfo;

/**
 *
 */
class INET_API TcpLwipConnection
{
  protected:
    class Stats
    {
      public:
        Stats();
        ~Stats();
        void recordSend(const TCPSegment &tcpsegP);
        void recordReceive(const TCPSegment &tcpsegP);

      protected:
        // statistics
        cOutVector sndWndVector;   // snd_wnd
        cOutVector sndSeqVector;   // sent seqNo
        cOutVector sndAckVector;   // sent ackNo

        cOutVector rcvWndVector;   // rcv_wnd
        cOutVector rcvSeqVector;   // received seqNo
        cOutVector rcvAckVector;   // received ackNo (= snd_una)
    };

    // prevent copy constructor:
    TcpLwipConnection(const TcpLwipConnection&);

  public:
    TcpLwipConnection(TCP_lwIP &tcpLwipP, int connIdP, int gateIndexP,
            TCPDataTransferMode dataTransferModeP);

    TcpLwipConnection(TcpLwipConnection &tcpLwipConnectionP, int connIdP,
            LwipTcpLayer::tcp_pcb *pcbP);

    ~TcpLwipConnection();

    void sendEstablishedMsg();

    static const char* indicationName(int code);

    void sendIndicationToApp(int code);

    void listen(Address& localAddr, unsigned short localPort);

    void connect(Address& localAddr, unsigned short localPort, Address& remoteAddr,
            unsigned short remotePort);

    void close();

    void abort();

    void sendSync(cPacket *msgP);

    void fillStatusInfo(TCPStatusInfo &statusInfo);

    void notifyAboutSending(const TCPSegment& tcpsegP);

    int send_data(void *data, int len);

    void do_SEND();

    INetStreamSocket* getSocket();

    void initStats();

  public:
    int connIdM;
    int appGateIndexM;
    LwipTcpLayer::tcp_pcb *pcbM;
    TcpLwipSendQueue *sendQueueM;
    TcpLwipReceiveQueue *receiveQueueM;
    TCP_lwIP &tcpLwipM;

  protected:
    long int totalSentM;
    bool isListenerM;
    bool onCloseM;

    Stats *statsM;
};

#endif

