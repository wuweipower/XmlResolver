```bash
vim /etc/yum/pluginconf.d/subscription-manager.conf
[main]
enabled=0
```

```
ifconfig eth0 up
参考https://blog.csdn.net/lly7858/article/details/9305015
```

```
vim /etc/yum.repos.d/xxx.repo
主要是修改baseurl
```
```
mkdir -p /www # 挂载的目录，随便起名
fdisk -l
fdisk /dev/sdb1
n
p
1


wq
mkfs.ext4 /dev/sdb1
echo "/dev/sdb1 /www ext4 defaults 0 0" >> /etc/fstab
mount 即可
```

```bash
lsblk # 查看目录结构，看是否有 没有挂载的磁盘
fdisk /dev/sdb # fdisk这个磁盘
m
n
p
1
然后设置起始和last
wq
lsblk -f 发现多了一个sdb1
mkfs -t ext4 /dev/sdb1 # 格式化磁盘分区
lsblk -f # 查看是否格式化为ext4类型
sudo mount /dev/sdb1 /home/new
df -Th # 查看是否挂载成功
blkid /dev/sdb1 # 查看UUID
echo “UUID/磁盘路径 挂载点 磁盘类型 defaults 0 0” >> /etc/fstab
```

连接虚拟机方便复制粘贴  
在虚拟机那里修改/etc/ssh/sshd_config 按照网上的步骤  
然后service sshd restart

yum install compat-libstdc++-33

libpam冲突问题  
```
yum reinstall libstdc++
yum reinstall pam
```

```
yum clean
yum distro-sync full
yum install libpam.so.0
ln -s libpam.so.0 libpam.so
```