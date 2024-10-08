# GEC6818
GEC6818模拟语音监控系统,通过GEC6818模拟语音监控系统的电路设计开发GEC6818语音监控系统系统的代码，并进行界面系统的调试和测试实现基本的语音控制与触碰控制，如灯光控制、相册、视频、音乐、监控模块的启动等功能。
# GGEC6818-Voice-monitoring-system

主要功能：基于GEC6818模拟语音监控系统实现语音或触碰的智能相册、音视频播放器、摄像头等功能

##### 1.实习目的

通过GEC6818模拟语音监控系统的电路设计开发GEC6818语音监控系统系统的代码，并进行界面系统的调试和测试实现基本的语音控制与触碰控制，如灯光控制、相册、视频、音乐、监控模块的启动等功能。

##### 2.环境搭建

1.虚拟机环境linux搭建

```c++
1).安装虚拟机并运行Ubuntu系统
安装 VMware虚拟机，解压Ubuntu18.04，运行VMware
2).设置共享文件夹
此共享文件夹是借助VMware 提供的 VMware Tools工具，实现宿主机与虚拟机两个系统之间的文件共享
3)使用Ubuntu终端和Linux命令交叉编译
```

2.交叉编译环境搭建

```c++
交叉开发是指在搭建了开发环境的宿主机上编写编译源代码文件，并生成目标平台何以运行的程序文件，并将该文件传输到目标平台运行的过程
宿主机：Windows（通过虚拟机 运行了 Ubuntu系统）
目标平台：GEC6818开发板，芯片采用的三星的S5P6818，8核处理
器（8*ARM Cortex-A53),系统主频1.8GHz，机载内存8G（有6G隐藏未挂载）
```

3.硬件接线：

```c++
5V-3A 电源是配置、DB9串口+USB转串口线
软件：SecureCRT + CH340串口驱动
2)安装串口驱动
3)使用SecureCRT链接开发板后台
3.开发板语音识别模块搭建安装
```

4.开发板 USB摄像头监控模块搭建安装

```c++
V4L2（Video4Linux2）是Linux操作系统中用于处理视频设备的内核框架。V4L2提供了一个统一的API，允许开发者在不同的硬件平台上编写通用的应用程序，从而访问、配置和操作视频设备，如摄像头、USB视频捕获卡等。
```

5.开发板摄像头音视频模块搭建安装

```c++
移植mplayer开源多媒体库:
MPlayer是一款开源的多媒体播放器，以GNU通用公共许可证发布。此款软件可在各主流作业系统使用，例如Linux和其他类Unix作业

系统、微软的视窗系统及苹果电脑的Mac OS X系统。MPlayer是基于命令行界面，在各作业系统可选择安装不同的图形界面。

因为linux下都是命令行的操作方式，所以对mplayer的各种操作都
是用命令来实现的，这次主要用的是它的slave工作方式（在slave模式下，MPlayer为后台运行其他程序，不再截获键盘事件， MPlayer会从标准输入读一个换行符（\n）分隔开的命令）。
```

项目主要框架图：

![](C:\Users\86166\AppData\Roaming\Typora\typora-user-images\image-20240920114900890.png)

##### 3.模块详细设计

###### 相册模块

```c++
相册左右滑动切换相片,上下滑动放缩图片,特定手势退出至主界面
使用mmap映射文件共享写入读取bmp文件实现单个图片显示，创建数组存放bmp图片文件地址完成图片切换，延迟函数与区块化显示图片制造百叶窗显示图片特效，定位图片中心偏移实现中心居中图片，色块数组存储模糊比例填充完成放大缩小图片
```

###### 音频播放模块

```c++
切歌，播放和暂停功能
madplay命令实现,管理madplay的主程序，包括播放，暂停播放，恢复播放，停止播放.播放音乐有个逻辑，就是如果是第一次播放的话，就要开始播放音乐，如果不是的话，就要继续播放音乐。上一首下一首功能，要防止数组越界，直接取模也是可以的。
```

###### 视频播放模块

```c++
使用mplayer实现视频播放,管理madplay的主程序，包括播放，暂停播放，恢复播放，停止播放.
```

###### 摄像头模块

```c++
V4L2（Video4Linux2）是Linux操作系统中用于处理视频设备的内核
框架。V4L2提供了一个统一的API，允许开发者在不同的硬件平台
上编写通用的应用程序，从而访问、配置和操作视频设备，如摄像头、
USB视频捕获卡等。
```

操作流程

- 打开设备
- 配置设备（采集的频率、图像宽高、图像格式）
- 在内核空间申请缓冲区队列
- 把申请好的缓冲列表----映射到用户空间
- 开始采集
- 从队列中拿出一个缓冲区
- 从用户空间中把缓冲区的数据取走
- 把缓冲区放回队列
- 停止采集

###### 语音识别模块

```c++
gec6818平台录音
录音模块是在gec6818上的Linux系统上运行的，目的是采集外部音
频的输入，进而生成一个wav格式的音频文件。因此对于这个模块来
说，输入是外部的声音信息，输出一个wav格式的音频文件。

socket传输文件
对于录音模块输出的音频文件，需要将其传输到主机平台上。因此，利用网络来实现文件的传输。对于文件传输模块来说，输入的是文件，输出的也是文件。

语音识别
因免费的语音识别库目前只能在x86平台运行，因此只能通过网络将设备采集到的音频文件传输到主机上，由主机来完成音频文件的解析，语音识别模块的输入的是wav格式的音频文件，输出的xml格式的数据，从而将其写成一个xml文件输出。

xml格式文本解析
对于语音识别模块输出的结果是一个xml文本格式的文件，因此需要对xml文本格式的数据进行解析，对于xml解析模块的输入是一个xml文本格式的文件，进而输出一堆需要的数据。

主题界面触控模块:
创建Touch函数监听lcd屏上压力感应,不同区域压力感应调用对应函数
```

###### LCD与蜂鸣器报警模块

```c++
语音控制灯光开启关闭，蜂鸣器开启关闭 
直接对开发板上 对应蜂鸣器和LED的文件写入控制数据即可
```

##### 4.实验结果与分析

能够实现触碰控制功能，如相册内图片缩放切换，视频播放，音乐播放等基本功能，进行语音数据采集与处理功能，控制相册打开，播放音乐、视频等语音唤醒功能。

主页面

![image-20240920120038232](https://i-blog.csdnimg.cn/direct/85e0658f254e4a2f803b084b7c7ea021.png)

视频功能测试

![image-20240920120111456](https://i-blog.csdnimg.cn/direct/ad2beed790c14dc08030fe87aeec79db.png)

展示相册功能

![image-20240920120130371](https://i-blog.csdnimg.cn/direct/a01b4648a6cc46a18544ff91c160238f.png)

展示音乐功能

![image-20240920120145216](https://i-blog.csdnimg.cn/direct/674d737cef8a467aa6d6d8956d3929d5.png)

