本工具用于删除Ramnit 病毒修改html文件名所添加的script脚本：<SCRIPT Language=VBScript>


作者：Zcc911
链接：https://www.zhihu.com/question/26686295/answer/202311756
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

某论坛网友曾经针对Ramnit病毒的EFG型变种分享过一篇逆向分析文章《rmnet感染型病毒分析》，大体分析了病毒的核心工作原理，腾讯反病毒实验室也发表过相关内容，本人现简要整理如下：一般情况下，病毒通过某种途径传播到了目标计算机内，此时病毒为子体状态（包含病毒母体代码）该子体中的病毒代码随着文件被调用而运行，在目录内释放病毒母体文件(一般文件名为：原文件名+Srv.exe)，然后调用WIN32API函数CreateProcess，创建该病毒的进程及其主线程，完成这些劫持了原程序流程的工作后，程序才跳转到原来的正常流程部分继续运行此时Srv.exe作为病毒母体，复制自身到“C:\Program Files\Microsoft\DesktopLayer.exe”，并劫持注册表中winlogon系统项的userinit，修改其值，实现病毒母体的自动运行DesktopLayer.exe劫持WIN32API函数ZwWriteVirtualMemory的正常工作流程，改变EIP寄存器的执行，然后调用WIN32API函数CreateProcess创建iexplore.exe进程，由于ZwWriteVirtualMemory是CreateProcess的内部函数，所以创建iexplore.exe进程的流程被篡改，病毒将自己的恶意可执行程序(PE)代码写入到iexplore.exe进程中并注入Shellcode，后者的任务是初始化PEiexplore.exe中PE开始运行，在入口处首先判断互斥体(Mutex)：“KyUffThOkYwRRtgPP”是否存在(避免重复感染操作)，如果存在便直接退出程序，否则创建6个线程，任务分工为：1. 每隔1秒执行一次，无限循环，判断winlogon.exe注册表项的userinit，确认启动的是否是病毒 母体(开机启动项中排查不出这种劫持方式)2. 访问Google或Bing、Yahoo等备选网站，获取当前时间3. 获取感染时间，保存到文件“dmlconf.dat”中4. 伪装成HTTPS协议(躲避杀毒软件的行为检测)，通过443端口连接C&C服务器“fget-career.com”，发送窃取到的本地数据5. 每隔30秒执行一次，无限循环，遍历全盘，感染*.exe、*.dll、*.htm和*.html文件；对于*.exe和*.dll文件，确保包含病毒母体代码的区段.rmnet得到注入；对于*.htm和*.html文件，确保包含病毒母体代码的VB脚本得到注入6. 遍历全部驱动器的根目录，确保全部磁盘内autorun.inf写入恶意代码，此代码可以随着autorun.inf的自动运行而执行病毒母体文章《rmnet感染型病毒分析》内的当时分享的病毒清理思路是：　　遍历每个进程的句柄表，捕获目标互斥体的所在进程，结束掉这个进程，然后自己创建这个互斥，这时病毒的感染操作可以视为被手动终止。接下来全盘搜索被感染的文件，进行修复。恢复原始入口点，删除.rmnet感染节区，修复文件校验和。参考链接：赛门铁克Ramnit病毒专杀工具(Symantec Ramnit Removal Tool) 2.4.4.3对rmnet蠕虫病毒的分析腾讯反病毒实验室赛门铁克携手欧洲刑警组织联合打击Ramnit僵尸网络犯罪团体

findstr /S /M /P VBScript *.html这就是你需要删的文件了。看样子它要在Program Files下自动生成svchost.exe然后执行，至于这个exe具体要干什么，估计是找html再写入script。重做系统之前连Program Files也一并干掉吧。

作者：知乎用户
链接：https://www.zhihu.com/question/26686295/answer/33660258
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

https://www.zhihu.com/question/26686295