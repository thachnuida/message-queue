#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define MSGSIZE 200 // kich thuoc 1 message
#define TYPE 1

typedef struct mymsgbuf{
	long type;
	char msg[MSGSIZE];
} message_buf;

GtkTextBuffer *buf_pheptoan;
GtkTextBuffer *buf_ketqua;

key_t key=1234;
//-------------------
GtkWidget* create_window();
GtkWidget * intro_window();
gboolean hide_window(GtkWidget *data);
gboolean show_window(GtkWidget *data);
void show_about();

void send_and_cal(GtkWidget *widget, gpointer data);
void read_result(GtkWidget *widget, gpointer data);

//----------------------

int main(int argc, char **argv){

    GtkWidget *window;
    GtkWidget *splash;

    gtk_init(&argc, &argv);

    // Hien thi splash screen
    splash = intro_window();
    gtk_widget_show_all(splash);
    // Dung
    g_timeout_add_seconds(2,hide_window,splash);

    // Hien thi window chinh
    window = create_window();
    //gtk_widget_show_all(window);
    g_timeout_add_seconds(2,show_window,window);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));

    gtk_main();
}

//---------------- [ Mo ta cac ham]  --------------------
GtkWidget* create_window(){
    GtkWidget *window;
    GtkWidget *vbox_main;
    GtkWidget *vbox_send;
    GtkWidget *vbox_rcv;
    GtkWidget *hbox;
    GtkWidget *hbox_bottom;

    GtkWidget *txt_pheptoan;
    GtkWidget *txt_ketqua;
    GtkWidget *scroll_pheptoan;
    GtkWidget *scroll_ketqua;

    GtkWidget *btn_send_msq;
    GtkWidget *btn_read_result;

    GtkWidget *img_top;
    GtkWidget *img_bottom;
    GtkWidget *img_left;

    // Tao nut thoat tu image + event_box
    GtkWidget *img_exit;
    GtkWidget *box_exit;

    // Tao nut goi thieu tu image + event_box
    GtkWidget *img_about;
    GtkWidget *box_about;

    // Thiet lap window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Hàng đợi thông điệp");
        gtk_window_set_default_size(GTK_WINDOW(window), 640, 500);
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_icon_from_file(GTK_WINDOW(window),"skin//icon.png",NULL);

    // Thiet lap Layout chinh
    vbox_main = gtk_vbox_new(FALSE, 5);
        gtk_container_add(GTK_CONTAINER(window), vbox_main);

    // Phan gioi thieu < TOP >
    img_top = gtk_image_new_from_file("skin//top.png");
        gtk_box_pack_start(GTK_BOX(vbox_main), img_top, TRUE, TRUE, 0);

    // Phan noi dung < CENTER >
    hbox = gtk_hbox_new(FALSE,3);
        gtk_box_pack_start(GTK_BOX(vbox_main), hbox, TRUE, TRUE, 0);

    // <LEFT>
    img_left = gtk_image_new_from_file("skin//left.png");
        gtk_box_pack_start(GTK_BOX(hbox), img_left, FALSE, FALSE, 0);

    // Thiet lap khung hien thi phep toan
    vbox_send = gtk_vbox_new(FALSE, 2);
        gtk_box_pack_start(GTK_BOX(hbox), vbox_send, TRUE, TRUE, 0);

    scroll_pheptoan = gtk_scrolled_window_new(NULL,NULL);
        gtk_box_pack_start(GTK_BOX(vbox_send), scroll_pheptoan, TRUE, TRUE, 0);

        txt_pheptoan = gtk_text_view_new();
            buf_pheptoan = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txt_pheptoan));
            gtk_widget_set_size_request(GTK_WIDGET(txt_pheptoan),205,300);
            gtk_container_add(GTK_CONTAINER(scroll_pheptoan), txt_pheptoan);


    btn_send_msq = gtk_button_new_with_label("Đọc các phép toán");
        g_signal_connect( G_OBJECT(btn_send_msq), "clicked", G_CALLBACK(send_and_cal), NULL);
        gtk_box_pack_start(GTK_BOX(vbox_send), btn_send_msq, TRUE, TRUE, 0);

    // Thiet lap khung hien thi ket qua
    vbox_rcv = gtk_vbox_new(FALSE,2);
        gtk_box_pack_start(GTK_BOX(hbox), vbox_rcv, TRUE, TRUE, 0);

    scroll_ketqua = gtk_scrolled_window_new(NULL,NULL);
        gtk_box_pack_start(GTK_BOX(vbox_rcv), scroll_ketqua, TRUE, TRUE, 0);

        txt_ketqua = gtk_text_view_new();
            buf_ketqua = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txt_ketqua));
            gtk_widget_set_size_request(GTK_WIDGET(txt_ketqua),205,300);
            gtk_container_add(GTK_CONTAINER(scroll_ketqua), txt_ketqua);

    btn_read_result = gtk_button_new_with_label("Xem file kết quả");
        g_signal_connect( G_OBJECT(btn_read_result), "clicked", G_CALLBACK(read_result), NULL);
        gtk_box_pack_start(GTK_BOX(vbox_rcv), btn_read_result, TRUE, TRUE, 0);
    // Ket thuc phan noi dung

    // Phan <BOTTOM>
    hbox_bottom = gtk_hbox_new(FALSE,0);
        gtk_box_pack_start(GTK_BOX(vbox_main), hbox_bottom, TRUE, TRUE, 0);

    img_bottom = gtk_image_new_from_file("skin//bottom.png");
        gtk_box_pack_start(GTK_BOX(hbox_bottom), img_bottom, TRUE, TRUE, 0);
    //Nut gioi thieu
    box_about = gtk_event_box_new();
        img_about = gtk_image_new_from_file("skin//about.png");
            gtk_container_add(GTK_CONTAINER(box_about), img_about);
        gtk_widget_set_events(box_about, GDK_BUTTON_PRESS_MASK);
        gtk_box_pack_start(GTK_BOX(hbox_bottom), box_about, TRUE, TRUE, 0);
        g_signal_connect( G_OBJECT(box_about), "button_press_event", G_CALLBACK(show_about), NULL);

    //Nut thoat
    box_exit = gtk_event_box_new();
        img_exit = gtk_image_new_from_file("skin//exit.png");
            gtk_container_add(GTK_CONTAINER(box_exit), img_exit);
        gtk_widget_set_events(box_exit, GDK_BUTTON_PRESS_MASK);
        gtk_box_pack_start(GTK_BOX(hbox_bottom), box_exit, TRUE, TRUE, 0);
        g_signal_connect( G_OBJECT(box_exit), "button_press_event", G_CALLBACK(gtk_main_quit), NULL);

    // ket thuc <BOTTOM>

    return window;

}

GtkWidget * intro_window(){
    GtkWidget *window, *img;

    // Thiet lap window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Giới thiệu");
        gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
        gtk_window_set_type_hint(GTK_WINDOW(window),GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_container_set_border_width(GTK_CONTAINER(window), 5);
        gtk_window_set_opacity(GTK_WINDOW(window),0.9);
        gtk_window_set_icon_from_file(GTK_WINDOW(window),"skin//icon.png",NULL);

    // Them hinh vao window
    img = gtk_image_new_from_file("skin//intro.png");
        gtk_container_add(GTK_CONTAINER(window), img);

    return window;
}

gboolean hide_window(GtkWidget *data){
     gtk_widget_hide(data);
     return FALSE;
}

gboolean show_window(GtkWidget *data){
    gtk_widget_show_all(data);
    return FALSE;
}

void show_about(){
    GtkWidget *about;
    about = intro_window();
    gtk_window_set_type_hint(GTK_WINDOW(about),GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_widget_show_all(about);
}

void send_and_cal(GtkWidget *widget, gpointer data){
    pid_t pid;
    pid = fork();

    if(pid>0){
        /* Tien trinh cha - Doc cac phep toan va goi vao message queue*/

        // Xoa sach textbox phep toan
        gtk_text_buffer_set_text( buf_pheptoan, "", 0);

        FILE *f = fopen("pheptoan.txt","r");
        message_buf mbuf;
        int msqid ;
        char *enter;

        if((msqid= msgget(key,IPC_CREAT | 0666))<0)	perror("[send]msgget");
        else{
            // Goi cac phep toan vao hang doi
            mbuf.type = TYPE;
            while( !feof(f) ){

                // Doc tung bieu thuc theo hang
                strcpy(mbuf.msg,"");
                fgets(mbuf.msg,200,f);

                enter = strchr(mbuf.msg,'\n');
                if(enter!=NULL) *enter='\0';

                if(strlen(mbuf.msg)>0){
                    if(msgsnd(msqid, &mbuf, sizeof(mbuf),IPC_NOWAIT)<0) perror("Msgsnd");
                    else{
                         gtk_text_buffer_insert_at_cursor( buf_pheptoan, mbuf.msg, strlen(mbuf.msg));
                         gtk_text_buffer_insert_at_cursor( buf_pheptoan, "\n", 1);
                    }
                }

            }
        }
        // Goi message ket thuc
        strcpy(mbuf.msg, "end");
        if(msgsnd(msqid, &mbuf, sizeof(mbuf),IPC_NOWAIT)<0) perror("Msgsnd");
        else{
            gtk_text_buffer_insert_at_cursor( buf_pheptoan, mbuf.msg, strlen(mbuf.msg));
        };

        fclose(f);
    }
    else{
        /* Tien trinh con */
        execlp("./tinhtoan",NULL);
        // Neu exe co loi, in ra ma loi
        perror("execlp");
        _exit(-1);
    }

}

void read_result(GtkWidget *widget, gpointer data){
    FILE *f = fopen("ketqua.txt","r");
    char line[200];
    char *enter;
    // Xoa sach textbox ket qua
    gtk_text_buffer_set_text( buf_ketqua, "", 0);

    while( !feof(f) ){
        strcpy(line,"");
        fgets(line,200,f);
        enter = strchr(line,'\n');
        if(enter!=NULL) *enter='\0';

        if(strlen(line)>0){
            gtk_text_buffer_insert_at_cursor( buf_ketqua, line, strlen(line));
            gtk_text_buffer_insert_at_cursor( buf_ketqua, "\n", 1);
        }
    }
    fclose(f);
}
