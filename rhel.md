```bash
vim /etc/yum/pluginconf.d/subscription-manager.conf
[main]
enabled=0
```

```
vim /etc/yum.repos.d/xxx.repo
主要是修改baseurl
```

连接虚拟机方便复制粘贴  
在虚拟机那里修改/etc/ssh/sshd_config 按照网上的步骤  
然后service sshd restart

yum install compat-libstdc++-33
 
libpam冲突问题  
```
yum clean
yum distro-sync full
yum install libpam.so.0
ln -s libpam.so.0 libpam.so
```