message-queue
=============

Message queue example with GTK UI in ubuntu (Code in code block)


#Kiến thức:
- IPC identifier. 
- Message buffer

#Lập trình giao diện với GTK trong code::Block

Cai dat thu vien GTK
--------------------

sudo apt-get install libgtk2.0-dev

Sử dụng: code::BLOCK
--------------------
vào : Setting / complier and Debugger
Complier setting / other options
them vao:  `pkg-config --cflags gtk+-2.0`  ( có dấu nháy ở 2 đầu )
Linker Setting / other option
	them vao: `pkg-config --libs gtk+-2.0`

xem các bài huớng dẫn ở đây:
----------------------------
http://zetcode.com/tutorials/gtktutorial/


Error GTK
----------
Why does this strange 'x io error' occur when I fork() in my GTK+ app? [GTK 2.x]
This is not really a GTK+ problem, and the problem is not related to fork() either. If the 'x io error' occurs then you probably use the exit() function in order to exit from the child process.
When GDK opens an X display, it creates a socket file descriptor. When you use the exit() function, you implicitly close all the open file descriptors, and the underlying X library really doesn't like this.
The right function to use here is _exit().
http://library.gnome.org/devel/gtk-faq/stable/x505.html
