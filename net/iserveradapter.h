#ifndef NET_ISERVERADAPTER_H
#define NET_ISERVERADAPTER_H

#ifndef UNUSED
#define UNUSED(v) ((void)(v))
#endif

namespace net {

class IServerAdapter {
friend class Connection;
friend class TCPConnection;
public:
// 单个packet回调
virtual IPacketHandler::HPRetCode handlePacket(Connection *connection, Packet *packet) = 0;
// 批量packet回调
virtual bool handleBatchPacket(Connection *connection, PacketQueue &packetQueue) {
UNUSED(packetQueue);
UNUSED(connection);
return false;
}
// 构造函数
IServerAdapter() {
_batchPushPacket = false;
}
// 析构函数
virtual ~IServerAdapter() {}
// setBatch()
void setBatchPushPacket(bool value) {
_batchPushPacket = value;
}
private:
bool _batchPushPacket;          // 批量post packet
};
}

#endif /*ISERVERADAPTER_H*/