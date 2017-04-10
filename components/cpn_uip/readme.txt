核心采用 contiki-2.6 内置的 uIP. 主要重写了传输层和应用层.
使其代码书写风格更接近于ezos, 让网络应用层的代码更灵活和易于理解.



目录分层说明

    apps  - 常用应用, 包括 DHCP, DNS, SMTP, 可在 uip_conf.h 使能后, 直接使用
          - 应用范例, 包括如 TCP服务器, TCP客户端, UDP服务器, UDP客户端, WEB服务器

    tcpip - 传输层, 将 uIP 的底层屏蔽掉, 直接对应用层提供统一的函数接口
          - uip_sys.c 让 uIP 完成基础的收发功能.
          - tcp_udp.c 对外提供 TCP/UDP SOCKET 管理函数
          - socket.c  提供 TCP/UDP SOCKET 收发处理函数
          - uip_lib.c 提供实用的操作函数供上层应用调用

    uip   - 网络层/传输层, uIP的核心代码, 仅作必要的修改, 并标注了 "by mousie @"
          - 此版本的uIP已支持IPv6, 但本系统没有使用!!!

    chips - 数据链路层, 网络芯片的驱动文件
          - tapdev.c 屏蔽各个底层驱动的差异性, 给uIP系统提供统一的函数接口
