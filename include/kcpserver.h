/*
 * File:   kcpserver.h
 * Author: axiezhou
 *
 * Created on 2016/10/20
*/

#ifndef __KCPSERVER_H__
#define __KCPSERVER_H__

#include <sys/time.h>
#include <string>
#include <map>

#include "kcpsession.h"

inline IUINT64 iclock()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    IINT64 value = ((IINT64)time.tv_sec) * 1000 + (time.tv_usec / 1000);
    return value;
}

typedef void(*kcp_recv_cb_func)(int, const char*, int);
typedef void(*session_kick_cb_func)(int);

struct KCPOptions
{
    int port;
    int keep_session_time;
    kcp_recv_cb_func recv_cb;
    session_kick_cb_func kick_cb;

    KCPOptions();
};

class KCPServer
{
public:
    friend class KCPSession;

public:
    KCPServer(const KCPOptions& options);
    ~KCPServer();

    bool Start();
    void Update();
    bool Send(int conv, const char* data, int len);
    void KickSession(int conv);
    bool SessionExist(int conv) const;

    const std::string& Error() const;

private:
    bool UDPBind();
    void Clear();
    KCPSession* GetSession(int conv);
    void DoOutput(const KCPAddr& addr, const char* data, int len);
    void UDPRead();
    void SessionUpdate();
    void OnKCPRevc(int conv, const char* data, int len);

    KCPOptions options_;
    int fd_;
    std::string error_;
    std::map<int, KCPSession*> sessions_;
    IUINT64 current_clock_;
};

#endif