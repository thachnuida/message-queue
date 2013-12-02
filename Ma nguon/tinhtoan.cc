#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include<iostream>
#include<stack>
#include<list>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define TOAN_HANG 	0
#define CONG 		1
#define TRU 		2
#define NHAN		3
#define CHIA		4
#define MO_NGOAC 	5
#define DONG_NGOAC 	6
#define SIN			7
#define COS			8
#define TANG		9
#define COTANG		10
#define LUY_THUA	11

// Gan ngoac ( co do uu tien = 0 de khi cac toan tu vao ngan xep gap (
// se khong lay ngoac ra. (Do tinh chat cua t.toan chuyen ve hau to)
#define DO_UU_TIEN_NGOAC 	0
#define DO_UU_TIEN_CONG 	1
#define DO_UU_TIEN_NHAN 	2
#define DO_UU_TIEN_LUY_THUA	3
#define DO_UU_TIEN_HAM_1_BIEN 4

#define MSGSIZE 200 // kich thuoc 1 message
#define TYPE 1


typedef struct mymsgbuf{
	long type;
	char msg[MSGSIZE];
} message_buf;

typedef struct PHANTU{
	int  loai;
	int  uuTien;
	char giaTri[10];
} phanTu;

// Tinh gia tri cua bieu thuc hau to
float tinhHauTo(list<phanTu> &bieuThuc);

// Chuyen bieu thuc trung to sang hau to.
list<phanTu> hauTo(list<phanTu> &bieuThuc);

// Tach chuoi bieu thuc thanh cac phan tu rieng biet
list<phanTu> tachBieuThuc(string bieuThuc);


// Them dau space vao truoc va sau str tim thay trong bieu thuc
void themSpace(string &bieuThuc, char *str);

key_t key=1234;

void msq2file();
float tinhToan(const string bieuThuc);


int main(){
    msq2file();
    return 0;
}

void msq2file(){
	FILE *f = fopen("ketqua.txt","w");
	message_buf mbuf;
	int msqid;
	int ok=1;
	float kq;
	string bieuThuc;
	if((msqid = msgget(key, 0666))<0) perror("[read]msgget") ;
	else{
        while(ok){
            if(msgrcv(msqid, &mbuf, sizeof(mbuf),TYPE,0)<0)  perror("msgrcv"); // Doc ko duoc
            else
                if(strcmp(mbuf.msg,"end")==0) ok=0; // Doc duoc message ket thuc
                else {
                    bieuThuc = mbuf.msg;
                    kq  = tinhToan(bieuThuc);
                    if(kq - (int)kq !=0 ) fprintf(f,"%s=%f\n", mbuf.msg,kq);
                    else fprintf(f,"%s=%d\n", mbuf.msg,(int)kq);
                }
        };
        // xoa bo hang doi
        msgctl(msqid,IPC_RMID,NULL);
	}

	fclose(f);
	printf("\nKet thuc viec doc msq va ghi ra file ketqua.txt");

}

float tinhToan(const string bieuThuc){
    list<phanTu> l;
    l = tachBieuThuc(bieuThuc);
    l = hauTo(l);

    return tinhHauTo(l);
}

void themSpace(string &bieuThuc, char *str){
	int	i = 0;
	i=bieuThuc.find(str,0);
	while( i != string::npos){
		bieuThuc.insert(i," ");
		bieuThuc.insert(i+2," ");
		i=bieuThuc.find(str,i+2);
	}
};

list<phanTu> tachBieuThuc(string bieuThuc){
	list<phanTu> lBieuThuc;

	// Them dau space truoc va sau cac dau + - * / ( ) ^
	// Them dau space vao truoc bieu thuc,tranh truong hop dau - dau chuoi,thay the tai -1(sai)
	bieuThuc.insert(0," ");
	themSpace(bieuThuc,"+");
	themSpace(bieuThuc,"-");
	themSpace(bieuThuc,"*");
	themSpace(bieuThuc,"/");
	themSpace(bieuThuc,"(");
	themSpace(bieuThuc,")");
	themSpace(bieuThuc,"^");

	// Lay lai cac so am bi nham voi dau -
	// Khi them khoang trang 2 ben dau -, chuong trinh co the nham dau - voi dau
	// bieu thi gia tri am
	// Gia tru bieu thi cho dau am, truoc no se la cac phep tinh, hoac ( hoac phep ^
	int i = bieuThuc.find("-",0), j;
	while(i!=string::npos){
		j=i-1;
		while(bieuThuc[j]==' ') j--;
		switch(bieuThuc[j]){
			case '+':
			case '-':
			case '*':
			case '/':
			case '(':
			case '^':
				// dau - cua so am
				// xoa space giua dau - va so sau no
				j=i+1;
				while(bieuThuc[j]==' ')	bieuThuc.erase(j,1);
		}
		i = bieuThuc.find("-",i+1);
	}

	// Tach cac toan tu, toan hang dua vao danh sach

	char *value = NULL;
	char str[200];
	strcpy(str,bieuThuc.c_str());
	phanTu ptu;

	value = strtok(str," ");
	i = 0;
	while(value!=NULL){
		if((value[0]>='0' && value[0]<='9') || (value[0]=='-' && value[1]>='0' && value[1]<='9'))	ptu.loai = TOAN_HANG;
		// Phan loai toan tu
		else{
			switch(value[0]){
				case '+':
					ptu.loai = CONG;
					ptu.uuTien = DO_UU_TIEN_CONG; break;
				case '-':
					ptu.loai = TRU;
					ptu.uuTien = DO_UU_TIEN_CONG; break;
				case '*':
					ptu.loai = NHAN;
					ptu.uuTien = DO_UU_TIEN_NHAN; break;
				case '/':
					ptu.loai = CHIA;
					ptu.uuTien = DO_UU_TIEN_NHAN; break;
				case '^':
					ptu.loai = LUY_THUA;
					ptu.uuTien = DO_UU_TIEN_LUY_THUA; break;
				case '(':
					ptu.loai = MO_NGOAC;
					ptu.uuTien = DO_UU_TIEN_NGOAC; break;
				case ')':
					ptu.loai = DONG_NGOAC; break;
				default:
					if(!strcmp(value,"sin"))	ptu.loai = SIN;
					else if(!strcmp(value,"cos"))	ptu.loai = COS;
					else if(!strcmp(value,"tg"))	ptu.loai = TANG;
					else if(!strcmp(value,"cotg"))	ptu.loai = COTANG;
					ptu.uuTien = DO_UU_TIEN_HAM_1_BIEN;
			}// end: switch
		} // end: phan loai toan tu

		strcpy(ptu.giaTri, value);
		lBieuThuc.push_back(ptu);

		value = strtok(NULL," ");
	}

	return lBieuThuc;
}

list<phanTu> hauTo(list<phanTu> &bieuThuc){
	stack<phanTu> s;
	list<phanTu> ketQua;
	list<phanTu>::iterator ptDuyet;

	for(ptDuyet = bieuThuc.begin(); ptDuyet != bieuThuc.end(); ptDuyet++){
		switch( (*ptDuyet).loai){
			case TOAN_HANG 	: ketQua.push_back(*ptDuyet); break;
			case MO_NGOAC	: s.push(*ptDuyet); break;
			case DONG_NGOAC	:
				// Lay cac toan tu cho den khi gap dau (
				while(s.top().loai != MO_NGOAC){
					ketQua.push_back(s.top());
					s.pop();
				}
				// Loai luon dau (
				s.pop();
				break;
			default:	// cac toan tu khac
				// Lay cac toan tu lon hon vao ket qua
				while( (!s.empty()) && (s.top().uuTien >= (*ptDuyet).uuTien) ){
					ketQua.push_back(s.top());
					s.pop();
				}
				s.push(*ptDuyet);
		}
	}

	// Duyet het bieu thuc, dua het toan tu con trong ngan xep vao ket qua
	while( !s.empty() ){
		ketQua.push_back(s.top());
		s.pop();
	}

	return ketQua;
}

float tinhHauTo(list<phanTu> &bieuThuc){
	stack<float> s;
	list<phanTu>::iterator ptDuyet;
	float so1,so2;


	for(ptDuyet = bieuThuc.begin(); ptDuyet!=bieuThuc.end(); ptDuyet++){
		if((*ptDuyet).loai == TOAN_HANG)  s.push(atof((*ptDuyet).giaTri));  // atof: ham chuyen chuoi sang double
		else if((*ptDuyet).uuTien == DO_UU_TIEN_HAM_1_BIEN){
			so1 = s.top();
			s.pop();

			switch((*ptDuyet).loai){
				case SIN: so1 = sin(so1); break;
				case COS: so1 = cos(so1); break;
				case TANG: so1 = tan(so1); break;
				case COTANG: so1 = 1/tan(so1); break;
			}
			s.push(so1);
		}
		else{
			// Thao tac voi cac toan tu 2 ngoi
			so2 = s.top(); s.pop();
			so1 = s.top(); s.pop();
			switch((*ptDuyet).loai){
				case CONG: so1+=so2; break;
				case TRU:	so1-=so2; break;
				case NHAN:	so1*=so2; break;
				case CHIA:	so1/=so2; break;
				case LUY_THUA: so1= pow(so1,so2); break;
			}
			s.push(so1);
		}
	}

	return s.top();

}

