lsblk # list block

```bash
sudo shutdown -h now # halt right away
sudo shutdown -h 5 # shutdown the system after 5 min
sudo shutdown -r new # reboot
```

wall msg # send message to all logged users in terminals

systemctl 丰富的工具

```bash
lspci -xvvv # list pci
lsusb # list usb
lshw # list hardware
```

find /lib/modules/$(uname -r) -type f -iname "*.ko" # where uname -r will return the name of the kernel image that’s currently running (to point “find” to the correct directory), the object type is “ file ” and the file extension is .ko

```bash
sudo modprobe module name # add module

sudo lshw -C network # show info about network card
```

```bash
#  create a new volume group
sudo vgcreate my-new-vg /dev/sdb2 /dev/sdb3

# you can use vg as part of a new logical volume:
sudo lvcreate -n my-new-lv my-new-vg

#  Finally, you can scan for all logical volumes on your system using lvscan:
sudo lvscan
```

libip6tc.so.0.1.0  
In this name “ lib ” tells us that this file is a library, “ ip6tc ” would be the package 
name, “so” identifies it as a dynamic library (“so” stands for shared object), and 0.1.0 is 
the package version. If this were a static library, there would be an “ a ” instead of the “so.”


You can use **ldd** to display the libraries that a particular package depends on

list all the libraries stored in the current cache, use  
`ldconfig -p`

local: dpkg
```bash
sudo dpkg -i pack_name_version_deb # install
sudo dpkg --uppack package
-r # remove
-P # purge
dpkg-reconfigure # reconfigure
dpkg -l # list all current installed packages
dpkg -s # display details of the specified package
```

repositories: apt
```bash
sudo apt-get install package
sudo apt-get intall -s # will display package dependencies without  installing
sudo apt-get remove
apt-cache show pkg package
```

```bash
myvar=hello
echo $myvar
export myvar
unset myvar
```

uname -a #  will display a great deal more about your kernel and installation.

通过 export 命令，我们可以将一个变量设置为环境变量，使其在当前 shell 进程中可被其他程序和子进程访问。

`source` 命令用于在当前 shell 环境中加载并执行指定的脚本文件。加载脚本文件意味着将脚本文件中的命令逐行执行，以便在当前 shell 环境中定义变量、设置别名、运行函数等。  
请注意，source 命令只是在当前 shell 环境中执行脚本文件，并不创建新的子进程。因此，脚本文件中的变量和操作会直接影响当前的 shell 环境。这在需要加载和运行一些配置文件或设置别名时非常有用


cat -n /etc/passwd

cut –d: –f1 /etc/passwd  
This reads the passwd file (which contains details of all existing user accounts). 
The -d flag sets the delimiter as colon (:), which means that, whenever a colon appears 
in the text, cut will think of it as the start of a new text field. The f1 means that you’re 
only interested in printing the first field of every line


```bash
expand -t 10 filename # convert every tab to ten spaces. 
unexpand -t 3 filename # convert every three spaces to tabs.

fmt -w 60 filename # start a new line after 60 characters. 
fmt -t filename # indent all but the first line of a paragraph

join column1 column2 # You can use join to merge data from two files with overlapping columns

paste column1 column2 # Paste is another tool for merging the contents of multiple files
paste -s column1 column2 #  Paste -s will print them sequentially:

Using uniq without any arguments will print only the first time a repeated line appears. Running it with the -u argument will only print lines that are never repeated
uniq

Using split will divide a single file into multiple files of a specified length.
split

tr
od
wc
sed
# To copy a partition called sdb1 to a USB drive called partitionbackup, run this:
dd if=/dev/sdb1 of=/media/usb/partitionbackup

tar cvf archivename.tar /home/myname/mydirectory/*
#  The first argument, c, means create, v tells tar to be verbose and print any necessary 
# updates to the screen, and f points to the file you are creating. The f must always be the 
# last argument and must be immediately followed by the name of the archive. The asterisk 
# (*) after the source directory address tells tar to compress all files and subdirectories it 
# finds in that directory.

ls | cpio -o > myarchivename.cpio #  The cpio archive tool works primarily through piped data.
ls | cpio -o | gzip > myarchivename.cpio.gz
```

```bash
> is stdout by defalut
1> stdout
>> add it to end
2> std error #  write any error messages (but NOT the file contents) to a file called errors.txt

# 只有std error的 2> 才会讲对应的输出传到指定文件
ps
ps -e 
ps aux
top

```

Type jobs if you want to list all the processes currently running in the background of this 
shell. Note that this command will give you a different PID: this time it’s the job number 
in relation to specifically this shell, which explains why you probably got the ID number 
of 1, rather than something in the high thousands. This is the ID you’ll use to edit the job 
status. To bring this copy job back to the foreground, type:

fg 1

where fg stands for foreground, and 1 is the job number you got through jobs . To suspend 
the job, hit Ctrl+z. From there you can, if you like, restart the job and send it back to the 
background to complete, using:

bg 1

```bash
kill
killall
```

You can assign each process with its own nice value between 19 and -20, where 19 is 
very nice, and -20 is just plain nasty

A process with a nice value of 19 is so nice, that it will yield its rights to a finite resource to just about any other process asking for it. If, on the other hand, it’s set to -20, a process will grab as much of the resource pie as it can, giving itself top priority. By default, most processes start with a neutral value of 0.

nice -10 apt-get install apache2

This will set the nice value to 10 (not negative 10), meaning it will, when possible, 
run when resource demand is generally low. A negative value is set this way:

nice --10 apt-get install apache2

If you know its PID, you can use renice to change the nice value of a process even 
once it’s already running: 

renice -10 -p 3745

With the -u argument, you can apply renice to all the processes associated with the 
named user: 

renice 10 -u tony 

Finally, you can also set the value for all processes owned by particular group:
renice 10 -g audio

While grep uses REGEX, its two cousins—egrep and fgrep—take different 
approaches: egrep (Extended grep, which should now be used as grep -E) has a larger 
list of characters it reads as metacharacters, while fgrep (grep -F) treats all characters as strictly literal and will often work more quickly as a result.


h Moves the cursor left one character. 
j Moves the cursor down one line. 
k Moves the cursor up one line. 
l Moves the cursor right one character. 
0 Moves the cursor to the start of the current line. 
dw Deletes the word that comes immediately after the cursor. 
d$ Deletes from the insertion point to the end of the line. 
dd Deletes the entire current line. 
D Deletes the rest of the current line from the cursor position. 
p Inserts the text deleted in the last deletion operation after current cursor location 
u Undoes the last action. 
yy Copies the line in which the cursor is located to the buffer. 
ZZ Saves the current file and ends vi. 
/ Search (for the term you enter)

