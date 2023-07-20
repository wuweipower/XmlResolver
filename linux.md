# linux
##  内核
内核主要负责  
1. 系统内存管理  
2. 软件程序管理
3. 硬件设备管理
4. 文件系统管理

### 系统内存管理
内核通过硬盘上称为交换空间(swap space)的存储区域来实现虚拟内存。内核在交换空间
和实际的物理内存之间反复交换虚拟内存中的内容。这使得系统以为自己拥有比物理内存更多的
可用内存。  

内存被划分为若干块,这些块称作页面(page)。内核会将每个内存页面置于物理内存或交
换空间中。然后,内核会维护一张内存页面表,指明哪些页面位于物理内存,哪些页面被交换到
了磁盘。  

内核会记录哪些内存页面正在使用中,自动把一段时间未访问的内存页面复制到交换空间区
域(称之为换出,swapping out)——即使还有内存可用。当程序要访问一个已被换出的内存页面
时,内核必须将物理内存中的另一个页面换出来为其腾出空间,然后从交换空间换入(swapping in)所请求的页面。  

### 软件程序管理
Linux 操作系统称运行中的程序为进程。  
内核创建了第一个进程(称为 init 进程)来启动系统中所有其他进程。当内核启动时,它会
将 init 进程载入虚拟内存。内核在启动其他进程时,会在虚拟内存中给新进程分配一块专有区域
来存储该进程用到的数据和代码。  
在 Linux 中,有多种 init 进程实现,目前最流行的是以下两种。  
1. SysVinit
2. systemd

### 硬件设备管理
任何 Linux 系统需要与之通信的设备都必须在内核代码中加入
其驱动程序。驱动程序相当于应用程序和硬件设备的“中间人”,允许内核同设备之间交换数据。
向 Linux 内核中插入设备驱动的方法有两种。  
- 将驱动程序编译入内核
- 将设备驱动模块加入内核

以前,插入设备驱动程序的唯一途径就是重新编译内核,开发人员提出了内核模块的概念,允许在无须重新编译内核的情况下将驱动程序插入运行中
的内核。

Linux 系统将硬件设备视为一种特殊文件,称为设备文件  
- 字符设备文件
- 块设备文件
- 网络设备文件

1. 字符设备文件对应于每次只能处理一个字符的设备,大多数类型的调制解调器和终端是作为
字符设备文件创建的。  
2. 块设备文件对应于每次以块形式处理数据的设备,比如硬盘驱动器。  
3. 网络设备文件对应于采用数据包发送和接收数据的设备,这包括网卡和一个特殊的环回设
备,后者允许 Linux 系统使用常见的网络编程协议同自身通信。  

Linux 会为系统的每个设备都创建一种称为“节点”的特殊文件。与设备的所有通信都是通
过设备节点完成的。每个节点都有一个唯一的数值对,以供 Linux 内核标识。数值对包括一个主设备号和一个次设备号。类似的设备会被划分到相同的主设备号下。次设备号用于标识主设备组下的某个特定设备。

### 文件系统管理
ext,ext2,ext3,ext4 是一种文件系统

Linux 内核采用虚拟文件系统(virtual file system,VFS)作为和各种文件系统交互的接口。这为 Linux 内核与其他类型文件系统之间的通信提供了一个标准接口。当文件系统被挂载和使用时,VFS 会在内存中缓存相关信息。

# shell
在图形化桌面出现之前,和 Unix 系统交互的唯一方式就是通过 shell 提供的文本命令行界面
(command line interface,CLI)。CLI 只允许输入文本,而且只能显示文本和基本图形输出.

Linux 系统启动时,会自动创建多个虚拟控制台。虚拟控制台是运行在 Linux 系统内存中的
终端会话,虚拟控制台终端的另一种替代方案是使用 Linux 图形化桌面环境中的终端仿真软件包。终端
仿真软件包会在桌面图形化窗口中模拟控制台终端,这个就是我们熟悉的命令行。

# bash shell
bash 手册的使用 man [你自己要查的内容]  
当你使用 man 命令查看命令手册页的时候,其中的信息是由分页程序(pager)来显示的。

分页程序是一种实用工具,能够逐页(或逐行)显示文本。你可以单击空格键进行翻页,或是使
用 Enter 键逐行查看。按 q 键退出手册页  

如果想使用多个命令选项,那么通常可以将其合并在一起。例如,要使用选项 -a 和
-b ,可以写作 -ab 。  

如果不记得命令名了,可以使用关键字来搜索手册页。语法为 man -k keyword

除了按照惯例命名的各段,手册页中还有不同的节。每节都分配了一个数字,从 1 开始,一
直到 9.

Linux 的路径中不使用驱动器盘符,Windows 会为每个物理磁盘分区分配一个盘符,每个分区都有自己的目录结构,用于访问存储在其中的文件。

Linux 则采用另一种方式。Linux 会将文件存储在名为虚拟目录(virtual directory)的单个目
录结构中。虚拟目录会将计算机中所有存储设备的文件路径都纳入单个目录结构。Linux 虚拟目录结构只包含一个称为根(root)目录的基础目录。根目录下的目录和文件会按照其访问路径一一列出.**要注意的是,路径本身并没有提供任何有关文件究竟存放在哪个物理磁盘中的信息。**

Linux 虚拟目录中比较复杂的部分是它如何来协调管理各个存储设备。我们称在 Linux 系统
中安装的第一块硬盘为根驱动器。根驱动器包含了虚拟目录的核心,其他目录都是从那里开始构建的。

**Linux 会使用根驱动器上一些特别的目录作为挂载点(mount point)。挂载点是虚拟目录中分配给额外存储设备的目录。**
Linux 会让文件和目录出现在这些挂载点目录中,即便它们位于其他物理驱动器中。

系统文件通常存储在根驱动器中,而用户文件则存储在其他驱动器中.

其他硬盘挂载到虚拟目录上
```bash
cd pwd .和..  
ls  
```

```bash
ls -F #用于区别文件和目录 /表示目录，*表示可执行文件
ls -a #显示当前目录下所有文件，包括以.开头的隐藏文件
ls -R #递归选项，列出子目录的所有文件
ls -l #产生长列表格式的输出，显示详细信息
ls -i #要查看文件或目录的 inode 编号
```

在长列表格式输出中，输出的第一行显示了为该目录中的文件所分配的总块数。后面每一个行格式如下  
- 文件类型,比如目录( d )、文件( - )、链接文件( l )、字符设备( c )或块设备( b )
- 文件的权限
- 文件的硬链接数
- 文件属主
- 文件属组
- 文件大小(以字节为单位)
- 文件的上次修改时间
- 文件名或目录名

```bash
touch # 创建空文件
ls 后面接上文件名的匹配模式即可
ls my_*
ls my_?
ls my_func
ls my_[a-i]
ls my_[!a]
```

```bash
cp src dst #如果目标地点有对应文件名，默认覆盖
cp -i src dst #提醒你是否要覆盖
cp one dir/ #会在dir目录下复制one
cp -R Documents/ NewDocuments/ #在执行 cp –R 命令之前,目录 NewDocuments 并不存在。它是随着 cp –R 命令被创建的,
```

**tab键可以命令行补全**

链接文件
- 软链接（符号链接）是一个实实在在的文件,该文件指向存放在虚拟目录结构中某个地方的另一个文件。**这两个以符号方式链接在一起的文件彼此的内容并不相同**。类似windows的快捷方式
  ```bash
  ln -s test_file slink_test_file
  ```

- 硬链接创建的是一个独立的虚拟文件,其中包含了原始文件的信息以及位置。两者就根本而言是同一个文件
  ```bash
  ln test_one hlink_test_one
  #只能对处于同一存储设备的文件创建硬链接。要想在位于不同存储设备的文件之间创建
  链接,只能使用符号链接。
  ```

在 Linux 中,重命名文件称为移动(moving),也可以移动文件  
```bash
mv from to
```

删除
```bash
rm -i file #-i 询问是否删除
rm -i f?l # 匹配删除
```

目录
```bash
mkdir dir
mdkir -p dir/subdir #-p 批量创建缺失的父目录
rmdir wrong_dir/ #默认之删除空目录
rm -r dir #删除dir下的文件和目录本身
#对于 rm 命令, -r 选项和 -R 选项的效果是一样的
```

查看文件类型
```bash
file filename
```

查看文件
```bash
cat file
cat -n file #给所有行加上行号
cat -b file #只给有文本的行加上行号
more file #分页查看，格键向前翻页Enter键逐行向前查看
less file
tail file # 默认最后十行
tail -n num file #显示组后num行
head 同理
head -num #直接在-后面写数字即可
```
</br>

# 更多bash
## 进程
ps只能显示某个时间点的信息
```bash
ps #默认只显示运行在当前终端中属于当前用户的那些进程
ps -ef #-e选项指定显示系统中运行的所有进程; -f 选项则扩充输出内容以显示一些有用的信息列。
ps -l # -l长信息
```
-f后的信息
- UID:启动该进程的用户。
- PID:进程 ID
- PPID:父进程的 PID(如果该进程是由另一个进程启动的)
- C:进程生命期中的 CPU 利用率。
- STIME:进程启动时的系统时间。
- TTY:进程是从哪个终端设备启动的。
- TIME:运行进程的累计 CPU 时间
- CMD:启动的程序名称

-l 选项之后多出的那几列
- F:内核分配给进程的系统标志
- S:进程的状态( O 代表正在运行; S 代表在休眠; R代表可运行,正等待运行; Z 代表僵化,已终止但找不到其父进程; T 代表停止)
- PRI:进程的优先级(数字越大,优先级越低)
- NI:谦让度(nice),用于决定优先级。
- ADDR:进程的内存地址。
- SZ:进程被换出时所需交换空间的大致大小
- WCHAN:进程休眠的内核函数地址。

top可以显示实时情况  

输出的第一部分显示的是系统概况:第一行显示了当前时间、系统的运行时长、登录的用户数以及系统的平均负载。平均负载有 3 个值,分别是最近 1 分钟、最近 5 分钟和最近 15 分钟的平均负载。

第二行显示了进程( top 称其为 task)概况:多少进程处于运行、休眠、停止以及僵化状态(僵化状态指进程已结束,但其父进程没有响应)

下一行显示了 CPU 概况。 top 会根据进程的属主(用户或是系统)和进程的状态(运行、空闲或等待)将 CPU 利用率分成几类输出。

紧跟其后的两行详细说明了系统内存的状态。前一行显示了系统的物理内存状态:总共有多少内存、当前用了多少,以及还有多少空闲。后一行显示了系统交换空间(如果分配了的话)的状态。

最后一部分显示了当前处于运行状态的进程的详细列表  

- PID:进程的 PID。
- USER:进程属主的用户名。
- PR:进程的优先级。
- NI:进程的谦让度。
- VIRT:进程占用的虚拟内存总量。
- RES:进程占用的物理内存总量。
- SHR:进程和其他进程共享的内存总量。
- S:进程的状态( D 代表可中断的休眠, R 代表运行,S 代表休眠, T 代表被跟踪或停止,Z 代表僵化 )
- %CPU:进程使用的 CPU 时间比例。
- %MEM:进程使用的可用物理内存比例。
- TIME+:自进程启动到目前为止所占用的 CPU 时间总量。
- COMMAND:进程所对应的命令行名称,也就是启动的程序名

键入 f 允许你选择用于对输出进行排序的字段,键入 d 允许你修改轮询间隔(polling interval),键入 q 可以退出 top 。

在 Linux 中,进程之间通过信号来通信.
```bash
kill pid #只能通过pid向进程发送term信号，尽可能终止程序
#如果要强行终止，则-s选项指定其他信号（用信号名或信号值）
kill -s HUP pid

pkill process_name #可以使用程序名代替pid，允许使用通配符
```

## 磁盘
在使用新的设备之前，需要将其放在虚拟目录中，这个工作成为挂载（mounting）

如今大部分linux发行版能自动挂载特性类型的可移动存储设备。

在默认情况下, mount 命令会输出当前系统已挂载的设备列表。但是,除了标准存储设备,较新版本的内核还会挂载大量用作管理目的的虚拟文件系统
```bash
mount -t ext4 # 查看ext4 文件系统的信息
mount -t vfat # 查看vfat 文件系统的挂载信息
```

mount名令提供了四部分信息  
- 设备文件名
- 设别在虚拟目录中的挂载点
- 文件系统类型
- 已挂载设备的访问状态

***
以下是手动挂载的命令
```bash
sudo mount -t type device directory 
```
参数type 指定了磁盘格式化所使用的文件系统类型。Linux 可以识别多种文件系统类型。如果与 Windows PC 共用移动存储设备,那么通常需要使用下列文件系统类型。

- vfat:Windows FAT32 文件系统,支持长文件名。
- ntfs:Windows NT 及后续操作系统中广泛使用的高级文件系统。
- exfat:专门为可移动存储设备优化的 Windows 文件系统。
- iso9660:标准 CD-ROM 和 DVD 文件系统

**多数 U 盘会使用 vfat 文件系统格式化。如果需要挂载数据 CD 或 DVD,则必须使用 iso9660**

eg
```bash
mount -t vfat /dev/sdb1 /media/disk
```
一旦存储设备被挂载到虚拟目录,root 用户就拥有了对该设备的所有访问权限,而其他用户
的访问则会被限制

移除可移动设备时,不能直接将设备拔下,应该先卸载
umount 命令支持通过设备文件或者挂载点来指定要卸载的设备
```
umount [directory | device ] 
```

df 命令会逐个显示已挂载的文件系统。与 mount 命令类似, df 命令会输出内核挂载的所有虚拟文件系统,因此可以使用 -t 选项来指定文件系统类型,进而过滤输出结果。

```bash
df -t ext4 -t vfat
```
输出结果如下
- 设备文件位置
- 包含多少以 1024 字节为单位的块
- 使用了多少以 1024 字节为单位的块
- 还有多少以 1024 字节为单位的块可用
- 已用空间所占的百分比
- 设备挂载点

最常用
```bash
df -h # human-readable
```

在默认情况下, du 命令会显示当前目录下所有的文件、目录和子目录的磁盘使用情况,并以磁盘块为单位来表明每个文件或目录占用了多大存储空间
```bash
du
```
行最左侧的数字是每个文件或目录所占用的磁盘块数。注意,这个列表是从目录层级的最底部开始,然后沿着其中包含的文件和子目录逐级向上的。

- -c :显示所有已列出文件的总大小。
- -h :按人类易读格式输出大小,分别用 K 表示千字节、M 表示兆字节、G 表示吉字节。
- -s :输出每个参数的汇总信息。

## 处理数据文件

```bash
sort file # 默认按照字典序排序，而且视数字为字符串
sort -n file # 按照数字排序
sort -m file # 将数字按照月排序
```

在对按字段分隔的数据(比如/etc/passwd 文件)进行排序时, -k 选项和 -t 选项非常方便。先使用 -t 选项指定字段分隔符,然后使用 -k 选项指定排序字段。例如,要根据用户 ID 对/etc/passwd 按数值排序,可以这么做:

```bash
sort -t ':' -k 3 -n /etc/passwd
```
```bash
du -sh * | sort -nr #本例中的管道命令( | )用于将 du 命令的输出传入 sort 命令
# -r 是reverse的意思
```

### 数据搜索
```bash
grep [options] pattern [file]
```
```bash
grep -v t file # -v 输出不匹配的行
grep -n t file # -n 显示匹配指定模式匹配的那些行的行号
grep -c t file # -c count 只想知道多少行含有匹配的模式
grep -e t -e f file # -e 指定多个匹配模式
```

### 数据压缩  
| 工具 | 文件拓展名 |
|  ----  | ----  |
| compress | .Z |
| gzip | .gz
| bzip2 | .bz2|
| xz | .xz|
| zip | .zip |

```
gzip # 用于压缩文件
gzcat # 用于查看压缩过的文本文件的内容。
gunzip #用于解压文件
```

```bash
gzip myfile # 就会把myfile生成为myfile.gz
gzip my* # 就会把所有以my开头的文件分别生成对应的gz 文件
```

### 数据归档

虽然 zip 命令能够很好地将数据压缩并归档为单个文件,但它并不是 Unix 和 Linux 中的标准归档工具。目前,Unix 和 Linux 中最流行的归档工具是 tar 命令。

```
tar function [options] object1 object2 ...
```

function  
| 操作 | 长选项 | 描述 |
|  ----  |  ----  | ---- |
| -A | --concatenate | 将一个tar归档文件追加到另一个tar归档文件末尾 |
| -c | --create | 创建新的 tar 归档文件|
| -r | --append | 将文件追加到 tar 归档文件末尾|
| -x | --extract | 从 tar 归档文件中提取文件 |
|-t | --list | 列出 tar 归档文件的内容|

option
| 选项 | 描述|
| ---- | ---- |
| -C dir | 切换到指定目录 |
| -f file | 将结果输出到文件(或设备) | 
| -v | 在处理文件时显示文件名 |
| -z | 将输出传给 gzip 命令进行压缩 |

eg
```bash
tar -cvf test.tar test/ test2/ # 创建归档文件
tar -tf test.tar #列出了(但不提取)tar 文件 test.tar 的内容。
tar -xvf test.tar # 该命令从 tar 文件 test.tar 中提取内容。如果创建的时候 tar 文件含有目录结构,则在当前目录中重建该目录的整个结构。
tar -zxvf filename.tgz
```

# 理解shell
```bash
which bash # 帮助我们找出bash shell的位置
```
多次在bash里面输入bash其实就是又创建了一个新的进程。

可以在单行中指定要依次运行的一系列命令。这可以通过命令列表来实现,只需将命令之间以分号( ; )分隔即可

```
pwd ; ls test* ; cd /etc ; pwd ; cd ; pwd ; ls my*
```
要想成为进程列表,命令列表必须将命令放入圆括号内:
```bash
(pwd ; ls test* ; cd /etc ; pwd ; cd ; pwd ; ls my*)
```

除了 ps 命令,也可以使用 jobs 命令来显示后台作业信息。 jobs 命令能够显示当前运行在后台模式中属于你的所有进程(作业)
```
jobs
ps -f
```

通过将进程列表置入后台,可以在子 shell 中进行大量的多进程处理。由此带来的一个好处是终端不再和子 shell 的 I/O 绑定在一起。
 
加个&就可以放在后台
```
(sleep 2 ; echo $BASH_SUBSHELL ; sleep 2)&
```

协程
协程同时做两件事:一是在后台生成一个子 shell,二是在该子 shell 中执行命令
```bash
coproc sleep 10
coproc My_Job { sleep 10; } # 设置名字
```
**用扩展语法,协程名被设置成了 My_Job 。这里要注意,扩展语法写起来有点儿小麻烦。你必须确保在左花括号( { )和内部命令名之间有一个空格。还必须保证内部命令以分号( ; )结尾。另外,分号和右花括号( } )之间也得有一个空格。**

## 外部命令与内建命令
### 外部命令
外部命令(有时也称为文件系统命令)是存在于 bash shell 之外的程序。也就是说,它并不属于 shell 程序的一部分。外部命令程序通常位于/bin、/usr/bin、/sbin 或/usr/sbin 目录中。  
可以使用 which 命令和 type 命令找到其对应的文件名

作为外部命令, ps 命令在执行时会产生一个子进程。

### 内建命令
与外部命令不同,内建命令无须使用子进程来执行。内建命令已经和 shell 编译成一体,作为 shell 的组成部分存在,无须借助外部程序文件来执行

可以使用 type 命令来判断某个命令是否为内建
要查看命令的不同实现,可以使用 type 命令的 -a 选项

bash shell 会跟踪你最近使用过的命令。你可以重新唤回这些命令,甚至加以重用。 history是一个实用的内建命令,能帮助你管理先前执行过的命令。

当输入 !! 时,bash 会先显示从 shell 的历史记录中唤回的命令,然后再执行该命令

可以在不退出 shell 的情况下强制将命令历史记录写入.bash_history 文件。为此,需要使用history 命令的 -a 选项:

你可以唤回历史记录中的任意命令。只需输入惊叹号和命令在历史记录中的编号即可:


# linux 环境变量
## 全局变量
全局环境变量对于 shell 会话和所有生成的子 shell 都是可见的。局部环境变量则只对创建它的 shell 可见。如果程序创建的子 shell 需要获取父 shell 信息,那么全局环境变量就能派上用场了。

系统环境变量基本上会使用全大写字母,以区别于用户自定义的环境变量。
```bash
# 查看环境变量
env
printenv 
# 显示个别环境变量的值
printenv HOME
echo $HOME
```
在变量名前加上 $ 可不仅仅是能够显示变量当前的值,它还能让变量作为其他命令的参数


在命令行中查看局部环境变量列表有点儿棘手。遗憾的是,没有哪个命令可以只显示这类变量。 set 命令可以显示特定进程的所有环境变量,既包括局部变量、全局变量,也包括用户自定义变量

设置局部变量
```bash
my_variable=Hello
my_variable="Hello World"
```
如果没有引号,则 bash shell 会将下一个单词( World)视为另一个要执行的命令。

设置全局变量
```
my_variable="I am Global now"
export my_variable
```
在定义并导出变量 my_variable 后, bash 命令生成了一个子 shell。在该子 shell 中可以正确显示出全局环境变量 my_variable 的值。子 shell 随后改变了这个变量的值。但是,这种改变仅在子 shell 中有效,并不会反映到父 shell 环境中。

子 shell 甚至无法使用 export 命令改变父 shell 中全局环境变量的值

删除环境变量
```bash
my_variable="I am going to be removed"
unset my_variable
```

## 设置path环境变量
当你在 shell CLI 中输入一个外部命令时,shell 必须搜索系统,从中找到对应的程序。 PATH 环境变量定义了用于查找命令和程序的目录

只需引用原来的 PATH 值,添加冒号( : ),然后再使用绝对路径输入新目录即可
```bash
PATH=$PATH:/home/christine/Scripts
```

### 环境变量持久化
对全局环境变量(Linux 系统的所有用户都要用到的变量)来说,可能更倾向于将新的或修改过的变量设置放在/etc/profile 文件中,但这可不是什么好主意。如果升级了所用的发行版,则该文件也会随之更新,这样一来,所有定制过的变量设置可就都没有了  

最好在/etc/profile.d 目录中创建一个以.sh 结尾的文件

alias 命令设置无法持久生效。你可以把个人的 alias 设置放在$HOME/.bashrc启动文件中,使其效果永久化。

## 数组变量
要为某个环境变量设置多个值,可以把值放在圆括号中,值与值之间以空格分隔:

```bash
mytest=(zero one two three four)
echo ${mytest[2]}
echo ${mytest[*]}
mytest[2]=seven
unset mytest[2]
```

***
# linux 文件权限
Linux 安全系统的核心是用户账户。每个能访问 Linux 系统的用户都会被分配一个唯一的用户账户

用户权限是通过创建用户时分配的用户 ID(user ID,UID)来跟踪的。UID 是个数值,每个用户都有一个唯一的 UID。但用户在登录系统时是使用登录名(login name)来代替 UID 登录的。登录名是用户用来登录系统的最长 8 字符的字符串(字符可以是数字或字母),同时会关联一个对应的密码

/etc/passwd

- 登录用户名
- 用户密码
- 用户账户的 UID(数字形式)
- 用户账户的组 ID(数字形式)
- 用户账户的文本描述(称为备注字段)
- 用户$HOME 目录的位置
- 用户的默认 shell

现在,绝大多数 Linux 系统将用户密码保存在单独的文件(称为 shadow 文件,位于/etc/shadow)中。只有特定的程序(比如登录程序)才能访问该文件

etc/shadow 文件中的每条记录共包含 9 个字段。
- 登录名,对应于/etc/passwd 文件中的登录名。
- 加密后的密码。
- 自上次修改密码后已经过去的天数(从 1970 年 1 月 1 日开始计算)
- 多少天后才能更改密码。
- 多少天后必须更改密码。
- 密码过期前提前多少天提醒用户更改密码。
- 密码过期后多少天禁用用户账户。
- 用户账户被禁用的日期(以从 1970 年 1 月 1 日到当时的天数表示)
- 预留给以后使用的字段

### 添加新用户
```bash
useradd -D # -D 选项显示了在命令行中创建新用户账户时,如果不明确指明具体值, useradd 命令所使用的默认值。
userdel -r test
```

### 修改用户
| 命令 | 描述 |
| ---- | ---- |
|usermod | 修改用户账户字段,还可以指定主要组(primary group)以及辅助组(secondary group)的所属关系|
| passwd |  修改已有用户的密码 |
| chpasswd | 从文件中读取登录名及密码并更新密码 |
| chage | 修改密码的过期日期 |
| chfn | 修改用户账户的备注信息 |
| chsh | 修改用户账户的默认登录 shell |


## linux 组
组权限允许多个用户对系统对象(比如文件、目录或设备等)共享一组权限

/etc/group
- 组名
- 组密码
- GID
- 属于该组的用户列表

```bash
/usr/sbin/groupadd shared
/usr/sbin/usermod -G shared rich # 向组添加rich
/usr/sbin/usermod -G shared test
```

## 文件权限

在ls -l后
```
总用量 60
-rw-rw-r-- 1 scutech scutech 18484 7月  18 08:58 git.md
-rw-rw-r-- 1 scutech scutech 25021 7月  18 15:54 linux.md
-rw-rw-r-- 1 scutech scutech  2151 7月  18 09:37 md.md
-rw-rw-r-- 1 scutech scutech    12 7月  18 14:36 README.md
drwxrwxr-x 2 scutech scutech  4096 7月  12 17:54 src
```
第一个字段就是描述文件和目录权限的编码.

第一个字符是对象类型
- \- 代表文件
- d 代表目录
- l 代表链接
- c 代表字符设备
- b 代表块设备
- p 代表具名管道
- s 代表网络套接字

之后是 3 组三字符的编码，按照顺序的
- r 代表对象是可读的
- w 代表对象是可写的
- x 代表对象是可执行的

如果没有某种权限,则在该权限位会出现连字符。这 3 组权限分别对应对象的 3 个安全级别。按顺序如下：
- 对象的属主
- 对象的属组
- 系统其他用户

### 默认文件权限

八进制模式的安全设置先获取 rwx 权限值,然后将其转换成 3 位(bit)二进制值,用一个八进制值来表示。在二进制表示中,每个位置代表一个二进制位。因此,如果读权限是唯一置位的权限,则权限值是 r-- ,转换成二进制值就是 100 ,代表的八进制值是 4 。

因此,八进制模式的值 664 代表属主和属组成员都有读取和写入的权限,而其他用户只有读取权限。

要把 umask 值从对象的全权限值(full permission)中减掉。对文件而言,全权限值是 666(所有用户都有读取和写入的权限);对目录而言,全权值则是 777 (所有用户都有读取、写入和执行权限)

所以,在上面的例子中,文件一开始的权限是 666 ,减去 umask 值 022 之后,剩下的文件权限就成了 644

使用 umask 命令指定其他的 umask 默认值
```
umask 026
```

### 修改权限
```bash
chmod options mode file
```

```
chmod 760 file
```

### 符号模式
[ugoa...]\[[+-=][rwxXstugo...]  
第一组字符定义了权限作用的对象。
- u 代表用户
- g 代表组
- o 代表其他用户
- a 代表上述所有

接下来的符号表示你是想在现有权限基础上增加权限( + )、移除权限( - ),还是设置权限( = )。

最后,第三个符号代表要设置的权限。你会发现,可取的值要比通常的 rwx 多。这些额外值如下

- X :仅当对象是目录或者已有执行权限时才赋予执行权限。
- s :在执行时设置 SUID 或 SGID。
- t :设置粘滞位(sticky bit)
- u :设置属主权限。
- g :设置属组权限。
- o :设置其他用户权限。

```
chmod o+r newfile
chmod u-x newfile
```

### 改变所属关系

Linux 为此提供了两个命令: chown 和 chgrp ,前者可以修改文件的属主,后者可以修改文件的默认属组。

```
chown options owner[.group] file
```

```
chown dan newfile
ls -l newfile
-rw-rw-r-- 1 dan rich 0 Sep 20 19:16 newfile

chown dan.shared newfile
ls -l newfile
-rw-rw-r-- 1 dan shared 0 Sep 20 19:16 newfile

chgrp shared newfile # chgrp 命令可以方便地修改文件或目录的默认属组
```

# 文件系统
## 文件系统的演变
### 早期
1.ext   
Linux 操作系统最初引入的文件系统叫作扩展文件系统(extended filesystem,简称 ext),它为 Linux 提供了一个基本的类 Unix 文件系统,使用虚拟目录处理物理存储设备并在其中以固定大小的磁盘块(fixed-length block)形式保存数据。

ext 文件系统使用 i 节点(inode)跟踪存储在虚拟目录中文件的相关信息。i 节点系统在每个物理存储设备中创建一个单独的表(称作 i 节点表)来保存文件信息。虚拟目录中的每个文件在i 节点表中有对应的条目。 

Linux 通过一个唯一的数值(称作 i 节点号)来引用 i 节点表中的 i 节点,这个值是创建文件
时由文件系统分配的。文件系统是通过 i 节点号而非文件名和路径来标识文件的.

2.ext2  
在保持与 ext 相同的文件系统结构的同时,ext2 在功能上做了扩展。
- 在 i 节点表中加入了文件的创建时间、修改时间以及最后一次访问时间。
- 允许的最大文件大小增至 2 TB,后期又增加到 32 TB。
- 保存文件时按组分配磁盘块。

ext2 文件系统也有限制。**如果系统在存储文件和更新 i 节点表之间发生了什么事情,则两者内容可能无法同步,潜在的结果是丢失文件在磁盘上的数据位置**

### 日志文件系统

日志文件系统为 Linux 系统增加了一层安全性。它放弃了之前先将数据直接写入存储设备再更新 i 节点表的做法,而是先将文件变更写入临时文件(称作日志)。在数据被成功写到存储设备和 i 节点表之后,再删除对应的日志条目。

1.ext3 文件系统  
ext3 文件系统是 ext2 的后续版本,支持最大 2 TB 的文件,能够管理 32 TB 大小的分区。在默认情况下,ext3 采用有序模式的日志方法,不过也可以通过命令行选项改用其他模式。ext3 文件系统无法恢复误删的文件,也没有提供数据压缩功能。

2.ext4 文件系统  
作为 ext3 的后续版本,ext4 文件系统最大支持 16 TiB 的文件,能够管理 1 EiB 大小的分区。在默认情况下,ext4 采用有序模式的日志方法,不过也可以通过命令行选项改用其他模式。另外还支持加密、压缩以及单目录下不限数量的子目录。先前的 ext2 和 ext3 也可以作为 ext4 挂载,以提高性能表现。

3.JFS  
JFS 文件系统采用的是有序模式的日志方法,只在日志中保存 i 节点数据,直到文件数据被写进存储设备后才将其删除。

4.ReiserFS 文件系统
5.XFS 文件系统

### 卷管理文件系统
就文件系统而言,日志技术的替代选择是一种称作写时复制(copy-on-write, COW)的技术。COW 通过快照(snapshot)兼顾了安全性和性能。在修改数据时,使用的是克隆或可写快照。修改过的数据并不会直接覆盖当前数据,而是被放入文件系统中另一个位置。

真正的 COW 系统仅在数据修改完成之后才会改动旧数据。如果从不覆盖旧数据,那么这种操作准确来说称作写时重定向 (redirect-on-write,ROW)。不过,通常都将 ROW 简称为 COW。

从一个或多个磁盘(或磁盘分区)创建的存储池提供了生成虚拟磁盘(称作卷)的能力。

1.ZFS 文件系统
2.Btrfs 文件系统
3.Stratis 文件系统

## 使用文件系统
### 创建分区
首先,需要在存储设备上创建可容纳文件系统的分区。分区范围可以是整个硬盘,也可以是部分硬盘以包含虚拟目录的一部分。

有时候,创建磁盘分区时最麻烦的地方就是找出 Linux 系统中的物理硬盘。Linux 采用了一种标准格式来为硬盘分配设备名称,在进行分区之前,必须熟悉这种格式。

- SATA 驱动器和 SCSI 驱动器:设备命名格式为/dev/sdx,其中字母 x 具体是什么要根据驱动器的检测顺序决定(第一个检测到的驱动器是 a,第二个是 b,以此类推)。
- SSD NVMe 驱动器:设备命名格式为/dev/nvmeNn#,其中数字 N 具体是什么要根据驱动器的检测顺序决定(从 0 起始)。#是分配给该驱动器的名称空间编号(从 1 起始)。
- IDE 驱动器:设备命名格式为/dev/hdx,其中字母 x 具体是什么要根据驱动器的检测顺序决定(第一个检测到的驱动器是 a,第二个是 b,以此类推)。

fdisk  
fdisk只能处理最大2TB的硬盘,fdisk 是一个交互式程序,允许你输入命令来逐步完成硬盘分区操作要启动 fdisk ,需要指定待分区的存储设备的名称,同时还必须有超级用户权限
```bash
whoami # 输出自己的用户名
fdisk /dev/sda
```

fdisk 命令有些简陋,不允许调整现有分区的大小。你能做的是删除现有分区,然后重新创建。

如果对分区做了改动,那么务必使用 w 命令将改动写入硬盘后再退出。如果不想保存修改内容,则直接使用 q 命令退出

gdisk  
如果存储设备要采用 GUID 分区表(GUID partition table,GPT),就要用到 gdisk :  
sudo parted  
parted 的卖点之一是允许调整现有的分区大小,所以可以很容易地收缩或扩大磁盘分区


所有的文件系统命令都允许通过不带选项的简单形式来创建默认的文件系统,但要求你拥有超级用户权限
```bash
sudo mkfs.ext4 /dev/sdb1
```

为分区创建好文件系统之后,下一步是将其挂载到虚拟目录中的某个挂载点,以便在新分区中存储数据。挂载点可以是虚拟目录中的任何位置。

```bash
mkdir /home/christine/part
sudo mount -t ext4 /dev/sdb1 /home/christine/part
lsblk -f /dev/sdb # 显示新近格式化过并挂载的分区。
```

### 文件系统的检查与修复

fsck 命令可以检查和修复大部分 Linux 文件系统类型


## 逻辑卷管理
数据只会越来越多。如果你在硬盘的标准分区上创建了文件系统,那么向已有的文件系统增添额外的存储空间多少是一种痛苦的体验。你只能在同一个物理硬盘的可用空间范围内调整
分区大小。如果硬盘上没有地方了,就得找一个更大的硬盘,手动将已有的文件系统转移到新硬盘。

这时候可以通过将另一块硬盘上的分区加入已有的文件系统来动态地添加存储空间。Linux逻辑卷管理器(logical volume manager,LVM)正是用来做这个的。它可以让你在无须重建整个文件系统的情况下,轻松地管理磁盘空间。

### LVM
1. 物理卷(physical volume, PV) 通过 LVM 的 pvcreate 命令创建。该命令指定了一个未使用的磁盘分区(或整个驱动器)由 LVM 使用。在这个过程中,LVM 结构、卷标和元数据都会被添加到该分区。
2. 卷组(volume group,VG)通过 LVM 的 vgcreate 命令创建。该命令会将 PV 加入存储池,后者随后用于构建各种逻辑卷。可以存在多个卷组。当你使用 vgcreate 将一个或多个 PV 加入 VG 时,也会同时添加卷组的元数据。被指定为 PV 的分区只能属于单个 VG。但是,被指定为 PV 的其他分区可以属于其他 VG。
3. 逻辑卷(logical volume,LV)通过 LVM 的 lvcreate 命令创建。这是逻辑卷创建过程的最终产物。LV 由 VG 的存储空间块 2 组成。你可以使用文件系统格式化 LV,然后将其挂载,像普通的磁盘分区那样使用。

### linux LVM
首次设置逻辑卷的步骤如下。  
(1) 创建物理卷。  
(2) 创建卷组。  
(3) 创建逻辑卷。  
(4) 格式化逻辑卷。  
(5) 挂载逻辑卷。  

```bash
#1. 创建PV
sudo pvcreate /dev/sdc1 /dev/sdd1 /dev/sde1
#2. 创建VG
sudo vgdisplay
sudo vgcreate vg00 /dev/sdc1 /dev/sdd1
#3. 创建LV
sudo lvcreate -L lg -v vg00
# 格式化和挂载LV
sudo mkfs.ext4 /dev/vg00/lvo10
mkdir my_LV
sudo mount -t ext4 /dev/vg00/lvo10 my_LV
```

扩大和收缩VG和LV  
Vgextend  
Vgreduce  
lvextend  
lvreduce  

# 安装软件

## 基于Debian系统
各种主流的 Linux 发行版都采用了某种形式的软件包管理系统来控制软件和库的安装。  

软件包存储在称为仓库(repository)的服务器上,可以利用本地 Linux 系统中的软件包管理器通过 Internet 访问,在其中搜索新的软件包,或是更新系统中已安装的软件包。

软件包通常存在依赖关系,为了能够正常运行,被依赖的包必须提前安装。软件包管理器会检测这些依赖关系并提前安装好所有额外的软件包

基于 Debian 的发行版(比如 Ubuntu 和 Linux Mint)使用的是 dpkg 命令  
基于 Red Hat 的发行版(比如 Fedora、CentOS 和 openSUSE)使用的是 rpm 命令  

dpkg 命令假定你已经将 DEB 包文件下载到本地 Linux 系统或是以 URL 的形式提供。但很多时候并非如此。通常情况下,你更愿意从所用的 Linux 发行版仓库中安装软件包。为此,需要使用 APT(advanced package tool)工具集

- apt-cache
- apt-get
- apt

apt 命令本质上是 apt-cache 命令和 apt-get 命令的前端。APT 的好处是不需要记住什么时候该使用哪个工具——它涵盖了软件包管理所需的方方面面

apt show 命令并不会指明软件包是否已经安装。它只根据软件仓库显示该软件包的详细信息。有一个细节无法通过 apt 获得,即与特定软件包相关的所有文件。为此,需要使用 dpkg  
命令:
`dpkg -L package_name`

也可以执行相反的操作,即找出特定的文件属于哪个软件包:
`dpkg --search absolute_file_name`

dpkg --search /bin/getfacl  
acl: /bin/getfacl  
输出表明文件 getfacl 属于 acl 软件包。

### 使用apt安装

```bash
apt search package_name # 在默认情况下，search 命令显示的是在名称或者描述中，包含搜索关键字的那些软件包

apt --names-only search zsh # 如果只是搜索软件包名称

apt install package_name
```

### apt 升级软件
```bash
sudo apt upgrade
apt full-upgrade # 如果必须删除某个软件包才能完成升级,可以使用以下命令:
```

### 卸载软件包
```bash
sudo apt purge zsh
```

注意,在 purge 的输出中, apt 警告我们 zsh-common 软件包存在依赖,不能自动删除,以免其他软件包还有需要。如果确定有依赖关系的软件包不会再有他用,可以使用 autoremove命令将其删除:

## 基于Red Hat系统
- yum
- zypper
- dnf yum的升级版

上面前端全部基于rpm。



### 列出已经安装的软件包

```bash
dnf list installed
#输出的信息可能在屏幕上一闪而过，最好是重定向到一个文件，然后使用more或者less查看
dnf list installed > installed_software
```

```bash
dnf list xterm # 查看特定软件包的详细信息
dnf list intalled xterm # 是否安装
```

```bash
dnf provides /usr/bin/gzip # 尝试找出哪个软件包安装了 /usr/bin/gzip
# 会检查本地仓库和默认的fedora仓库
```

### dnf 安装软件

```
dnf install package_name
sudo dnf install zsh
```

### dnf 升级软件
```bash
dnf list upgrades # 查看已经安装软件包的可用更新
dnf upgrade package_name # 升级特定软件包
dnf upgrade # 更新所有软件
```

### 卸载软件
```bash
dnf remove package_name
```

### 处理损坏的依赖关系

有时在安装多个软件包时,一个软件包的依赖关系可能会被另一个软件包搞乱。这称为依赖关系损坏(broken dependency)。  
如果你的系统出现了这种情况,可以先试试下列命令:
```
dnf clean all
# 然后尝试使用dnf upgrade
```

如果人解决不了
```
dnf repoquery --deplist package_name #该命令会显示指定软件包的所有依赖关系以及哪些软件包提供了这种依赖。只要知道软件包需要哪个库,就可以自行安装了
```

### RPM 仓库

```bash
dnf repolist # 查看当前拉取软件的仓库
```
如果没有对应的仓库，就需要编辑配置文件
```
/etc/yum.repos.d
/etc/dnf/dnf/conf
```

使用lftp下载
```
sudo apt install lftp
lftp scutech:dingjia@192.168.88.20
get 对应文件就行
```

## 使用容器管理软件
### snap

```bash
snap list # 查看已安装
snap find package_name
snap info package_name
snap install package_name
sudo snap remove package_name
```

### flatpak
Red Hat, CentOS, Fedora

```bash
flatpak list
flatpak search package_name
sudo flatpak install app_id
sudo flatpak uninstall app_id
```

## 源码安装
```bash
tar -Jxvf file.tar.xz
cd
ls
查看readme或者install文件来下载
#如果缺失
make # 注意要安装make和gcc这些
make install
```

# 编辑器
## vim
```bash
#安装基础版vim
sudo apt install vim
readlink -f /usr/bin/vi
```

三种模式
- 命令模式
- Ex模式
- 插入模式

刚打开要编辑的文件(或新建文件)时, vim 编辑器会进入命令模式(有时也称为普通模式)  

在插入模式中,vim 会将你在当前光标位置输入的字符、数字或者符号都放入缓冲区。按下i 键就可以进入插入模式。要退出插入模式并返回命令模式,按下 Esc 键即可  

在命令模式中,可以用方向键在文本区域中移动光标(只要 vim 能正确识别你的终端类型)。如果恰巧碰到了一个罕见的没有定义方向键的终端连接,也不是没有办法。vim 编辑器也有可用于移动光标的命令

- h 左移
- j 下
- k 上
- l 右
- PageDown(或 Ctrl+F):下翻一屏
- PageUp(或 Ctrl+B):上翻一屏
- G :移到缓冲区中的最后一行
- num G :移到缓冲区中的第 num 行
- gg :移到缓冲区中的第一行。

vim 编辑器的命令模式有个称作 Ex 模式的特别功能。Ex 模式提供了一个交互式命令行,可以输入额外的命令来控制 vim 的操作。要进入 Ex 模式,在命令模式中按下冒号键(:)即可

- q :如果未修改缓冲区数据,则退出。
- q! :放弃对缓冲区数据的所有修改并退出。
- w filename :将文件另存为其他名称。
- wq :将缓冲区数据保存到文件中并退出


可以使用 vim 的搜索命令轻松查找缓冲区中的数据。如果要输入一个查找字符串,可以按下正斜线(/)键。光标会“跑”到屏幕底部的消息行,然后显示出一个正斜线。在输入要查找的文本后,按下 Enter 键。vim 编辑器会执行下列三种操作之一。
- 如果要查找的文本出现在光标当前位置之后,则光标会跳到该文本出现的第一个位置。
- 如果要查找的文本未在光标当前位置之后出现,则光标会绕过文件末尾,出现在该文本所在的第一个位置(并用一条消息指明)。
- 输出一条错误消息,说明在文件中没有找到要查找的文本。如果要继续查找同一个单词,按/键,然后再按 Enter 键,或者按 n 键,表示下一个(next)。

Ex 模式的替换命令允许快速将文本中的一个单词替换成另一个单词。要使用替换命令,必须处于命令行模式下。替换命令的格式是 :s/old/new/ 。vim 编辑器会跳到 old 第一次出现的地方并用 new 来替换。可以对替换命令做一些修改来替换多处文本。
- :s/old/new/g :替换当前行内出现的所有 old 。
- :n,ms/old/new/g :替换第 n 行和第 m 行之间出现的所有 old 
- :%s/old/new/g :替换整个文件中出现的所有 old 。
- :%s/old/new/gc :替换整个文件中出现的所有 old ,并在每次替换时提示。

# 构建基础脚本
## 使用多个命令
在命令行中，执行多个命令，用;就可以执行了

## 创建shell脚本
在创建 shell 脚本文件时，必须在文件的第一行指定要使用的 shell，格式如下：  
`#!/bin/bash`

在普通的 shell 脚本中， # 用作注释行。shell 并不会处理 shell 脚本中的注释行。然而，shell脚本文件的第一行是个例外， # 后面的惊叹号会告诉 shell 用哪个 shell 来运行脚本。

在指明了 shell 之后，可以在文件的各行输入命令，每行末尾加一个换行符。之前提到过，注释可用 # 添加

```bash
chmod u+x test
./test
```

## 显示消息
```
echo "str"
echo str
# 如果想把字符串和命令输出显示在同一行中
echo -n "str"
```

## 使用变量
```
echo $VAR_NAME
#以下是自定义变量 中间不能存在空格
var1=10
var2="string"
可以使用 $ 来引用这些变量
```

## 命令替换
```shell
testing=`date`
或者
testing=$(date)
#变量 testing 保存着 date 命令的输出，然后会使用 echo 命令显示出该变量的值
```

## 重定向输入和输出
```shell
# 输出重定向
command > outputfile
#可以用双大于号（ >> ）来追加数据
command >> outputfile

#输入重定向
command < inputfile
#内联输入重定向 这种方法无须使用文件进行重定向，只需在命令行中指定用于输入重定向的数据即可
# 内联输入重定向运算符是双小于号（ << ）。除了这个符号，必须指定一个文本标记来划分输入数据的起止。
command << marker
eg: wc << EOF
```

## 管道
```shell
#无须将命令输出重定向至文件，可以将其直接传给另一个命令。这个过程称为管道连接（piping）。
command1 | command2
```

## 数学运算
```shell
expr 
# 除了expr 将数学运算结果赋给变量，可以使用 $ 和方括号（ $[ operation ] ）
# 中间要有空格
var1=$[1 + 1]

# 使用bc解决浮点数问题
variable=$(echo "options; expression" | bc)
var1=$(echo " scale=4; 3.44 / 5" | bc)

# 退出码
echo $?
exit
```

# 结构化命令
```shell
#if后面的命令成功执行then里面的代码才被执行
if command
then
  commands
else 
  commands
fi
# if可以嵌套
if command
  then
    if 
      then
    fi
fi
```

## test
test 命令可以在 if-then 语句中测试不同的条件。如果 test 命令中列出的条件成立，那么test 命令就会退出并返回退出状态码 0

```
if test condition
then
commands
fi
```
如果不写 test 命令的 condition 部分，则它会以非 0 的退出状态码退出并执行 else 代码

### 数值比较
```
num_1 -eq num_2 # num_1是否等于num_2
```

### 字符串比较

### 文件比较

## 复合条件测试
```
[ condition1 ] && [ condition2 ]
[ condition1 ] || [ condition2 ]
```

## if-then高级特性

```shell
# 在子 shell 中执行命令的单括号。
# (command)在 bash shell 执行 command 之前，会先创建一个子 shell，然后在其中执行命令
if (echo $BASH_SUBSHELL)

# 双括号命令允许在比较过程中使用高级数学表达式
# (( expression ))

# 双方括号命令提供了针对字符串比较的高级特性
# [[ expression ]]
```

## case
```shell
case variable in
pattern1 | pattern2) commands1;;
pattern3) commands2;;
*) default commands;;
esac
```

# 更多结构化命令

```shell
for var in list
do
commands
done
```

C 风格
```shell
for (( i=1; i <= 10; i++ ))
do
echo "The next number is $i"
done
```

while

```shell
while test command
do
other commands
done
```

```
break
continue
```

# 处理用户输入

bash shell 会将所有的命令行参数都指派给称作位置参数（positional parameter）的特殊变量。这也包括 shell 脚本名称。位置变量 ① 的名称都是标准数字： $0 对应脚本名， $1 对应第一个命令行参数， $2 对应第二个命令行参数，以此类推，直到 $9，在第 9 个位置变量之后，必须在变量名两侧加上花括号，比如 ${10} 。

记住，参数之间是以空格分隔的，所以 shell 会将字符串包含的空格视为两个参数的分隔符。要想在参数值中加入空格，必须使用引号（单引号或双引号均可）。

basename命令可以返回不包含路径的脚本名：