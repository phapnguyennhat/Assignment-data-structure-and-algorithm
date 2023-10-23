#include "main.h"
extern int MAXSIZE;
class imp_res : public Restaurant
{
	private:
		customer* positionX;
		customer* headCustomerInQueue;
		customer* order;
		int countInDesk;
		int countInQueue;

	private:
		bool prior(customer* cusj, customer* cusj_incr){
			// cusj-> energy> cusj_incr
			// neu bang thi stt cua j phai o truoc j_incr
			if(abs(cusj->energy)>abs(cusj_incr->energy)) return true;
			else if(abs(cusj->energy)==abs(cusj_incr->energy) && sttOfCus(cusj->name)< sttOfCus(cusj_incr->name) ) return true;
			return false;
		}
		int sttOfCus(string name){
			int res=0;
			customer* tmp=order;
			do{
				if(tmp->name==name) return res;
				tmp=tmp->next;
				res++;
			}
			while(tmp!=order);
			return -1;
		}
		customer* findCus(customer* head, string name){
			if(head==NULL) return NULL;
			customer* tmp=head;
			do{
				if(tmp->name==name) return tmp;
				tmp=tmp->next;
			}
			while(tmp!=head);
			return NULL;
		}
		void remove(customer *& head,customer* cus){
			// xóa phần tử cus trong danh sach lien ket (inDesk,queue,order)

			if(head==NULL||cus==NULL) return;
			countInDesk=(head==positionX)? countInDesk-1:countInDesk;
			countInQueue=(head==headCustomerInQueue)? countInQueue-1:countInQueue;
			// positionX=(head==positionX	&&cus->energy>0) ?cus->next:cus->prev; 
			// head=(head==cus)? head->next:head;
			if(head==positionX && cus->energy>0) positionX=cus->next;
			else if(head==positionX && cus->energy<0) positionX=cus->prev;
			else if(head==cus) head=head->next;
			cus->prev->next=cus->next;
			cus->next->prev=cus->prev;
			cus->next=nullptr;
			cus->prev=nullptr;
			delete cus;
			headCustomerInQueue=(countInQueue==0)? NULL:headCustomerInQueue;
			positionX=(countInDesk==0)? NULL:positionX;
			order=(countInDesk==0&&countInQueue==0)? NULL: order;

		}
		void insert_next(customer*& pos,customer* cus){
			if(pos==NULL){
				pos=cus;
				cus->next=cus;
				cus->prev=cus;
			}
			else{
				cus->prev=pos;
				cus->next=pos->next;
				pos->next->prev=cus;
				pos->next=cus;
			}
			// countInDesk=(pos==positionX)? countInDesk+1:countInDesk;
			// positionX=(pos==positionX)? cus:positionX;
			countInQueue=(pos==headCustomerInQueue)? countInQueue+1: countInQueue;
		}
		void insert_pre (customer*& pos,customer* cus){
			if(pos==NULL){
				pos=cus;
				cus->next=cus;
				cus->prev=cus;
			}
			else{
				cus->next=pos;
				cus->prev=pos->prev;
				pos->prev->next=cus;
				pos->prev=cus;
			}
			// positionX=(pos==positionX)? cus:positionX;
			// countInDesk=(pos==positionX)? countInDesk+1:countInDesk;
			countInQueue=(pos==headCustomerInQueue)? countInQueue+1: countInQueue;
		}
		customer* customerAt(customer* head,int index){
			for(int i=0;i<index;i++){
				head=head->next;
			}
			return head;
		}
		void swap(customer* cus1,customer* cus2){
			if(positionX==cus1) positionX=cus2;
			else if(positionX==cus2) positionX=cus1;
			string tmp=cus1->name;
			cus1->name=cus2->name;
			cus2->name=tmp;
			int e=cus1->energy;
			cus1->energy=cus2->energy;
			cus2->energy=e;
		}
		int inssort2(customer* head,int size,int incr){
			// sắp xếp theo thứ tự giảm dần của sublist
			// trả về số swap thực hiện
			int res=0;
			for(int i=incr;i<size;i+=incr){
				for(int j=i;j>=incr&& prior(customerAt(head,j),customerAt(head,j-incr));j-=incr){
					// swap
					res++;
					swap(customerAt(head,j),customerAt(head,j-incr));
				}
			}
			return res;
		}
		int shellsort(customer* head,int size){
			// sửa lại thứ tự
			int res=0;
			for(int i=size/2;i>2;i/=2){
				for(int j=0;j<i;j++){
					res+=inssort2(customerAt(head,j),size-j,i);
				}
			}
			res+=inssort2(head,size,1);
			return res;
		}
		void kickCustomer(bool isOanLinh,int countThuatSu,int countOanLinh)
		{
			// đuổi khách hàng từ vị khách mới đến cho đến kh đến sớm nhất
			int count=isOanLinh?countOanLinh:countThuatSu;
			if(order!=NULL){
				customer*tmp=order->prev;
				while(count!=0){
					bool check=(isOanLinh)? tmp->energy<0:tmp->energy>0;
					if(check){
						tmp->print();
						remove(headCustomerInQueue,findCus(headCustomerInQueue,tmp->name));
						remove(positionX,findCus(positionX,tmp->name));
						
						tmp=tmp->prev;
						remove(order,tmp->next);
						count--;
					}
					else tmp=tmp->prev;
				}
			
			}
				
		}
		void printSubList(int indexBegin,int size){
			// indexbegin la vi tri bat dau tinh tu position x
			// size kich thuoc sublist
			int indexMin=indexBegin;
			int minE=customerAt(positionX,indexBegin)->energy;
			for(int i=indexBegin;i<size+indexBegin;i++){
				int index=i%countInDesk;
				if(customerAt(positionX,index)->energy<minE){
					minE=customerAt(positionX,index)->energy;
					indexMin=index;
				}
			}
			int index=indexMin;

			do{
				bool check=(indexBegin+size)<=countInDesk?index>=indexBegin&&index<indexBegin+size:((index>=0&&index<(indexBegin+size)%countInDesk)||(index>=indexBegin&&index<countInDesk));
 				if(check){
					customerAt(positionX,index)->print();
				}
				index=(index+1)%countInDesk;
			}
			while(index!=indexMin);
		
		}
	public:	
		imp_res() {
			positionX=NULL;
			headCustomerInQueue=NULL;
			order=NULL;
			countInDesk=0;
			countInQueue=0;
		}
		~imp_res(){
			while(positionX!=NULL){
				remove(positionX,positionX);
			}
			while(headCustomerInQueue!=NULL){
				remove(headCustomerInQueue,headCustomerInQueue);
			}
			while(order!=NULL){
				remove (order,order);
			}
			
		}
		
		void RED(string name, int energy)
		{
			// reject energy equal 0
			if(energy==0) return;
			// thiên thượng điện hạ, duy ngã độc tôn
			// reject customer whose name is on the desk or the queue
			// if(positionX!=NULL){
			// 	customer* tmp=positionX;
			// 	do{
			// 		if(tmp->name==name) return;
			// 		tmp=tmp->next;
			// 	}
			// 	while(tmp!=positionX);
			// }
			// if(headCustomerInQueue!=NULL){
			// 	customer* tmp=headCustomerInQueue;
			// 	do{
			// 		if (tmp->name==name) return;
			// 		tmp=tmp->next;
			// 	}
			// 	while(tmp!=headCustomerInQueue);
			// }
			if(findCus(positionX,name)!=NULL) return;
			if(findCus(headCustomerInQueue,name)) return;
			if(countInDesk==MAXSIZE && countInQueue==MAXSIZE) return;

			
 
			// cout<<name<<" "<<energy<<endl;
			customer*cus=new customer(name,energy,nullptr,nullptr);
			customer*cusOrder=new customer(name ,energy,nullptr,nullptr);
			// đưa khách vào bàn hoặc vào hàng đợi
			if(countInDesk<MAXSIZE/2){
				// chen khach vao ban
				if(findCus(order,name)==NULL){
					insert_pre(order,cusOrder);		//push back
				}
				if(positionX==NULL||energy>=positionX->energy){
					// chieu dong ho
					insert_next(positionX,cus);
				}
				else if(positionX==NULL||energy<positionX->energy){
					// nguoc chieu dong ho
					insert_pre(positionX,cus);
				}
				
				positionX=cus;
				countInDesk++;
			}
			else{
				if(countInDesk==MAXSIZE && countInQueue<MAXSIZE){
				// them vao hang cho
					insert_pre(headCustomerInQueue,cus);
					if(findCus(order,name)==NULL){
						insert_pre(order,cusOrder);		//push back
					}

				}
				else if(countInDesk<MAXSIZE&&countInDesk>=MAXSIZE/2 ){
					// dua vao ban theo chien thuat 2
					if (findCus(order,name)==NULL){
						insert_pre(order,cusOrder);
					}
					int sub=0;
					customer* RES=positionX;
					customer* tmp=positionX;
					do
					{
						if (abs(energy - tmp->energy) > abs(sub))
						{
							sub = energy - tmp->energy;
							RES = tmp;
						}
						tmp = tmp->next;
					}
					while(tmp!=positionX);

					if(sub<0){
						insert_pre(RES,cus);
					}
					else{
						insert_next(RES,cus);
					}
					positionX=cus;
					countInDesk++;
				}
			}
		}
		void BLUE(int num)
		{
			// cout << "blue "<< num << endl;
			
			if(num>=countInDesk){
				customer* tmp=order;
				customer* cus;
				while(positionX!=NULL){
					cus=findCus(positionX,tmp->name);
					if(cus!=NULL){
						remove(positionX,cus);
						tmp=tmp->next;
						remove(order,tmp->prev);
					}
					else{
						tmp=tmp->next;
					}
				}
			
			}
			else{
				customer* tmp=order;
				customer* cus;
				while(num!=0){
					cus=findCus(positionX,tmp->name);
					if(cus!=NULL){
						remove(positionX,cus);
						tmp=tmp->next;
						remove(order,tmp->prev);
						num--;
					}
					else{
						tmp=tmp->next;
					}
				}
			}
			// them kh tu hang doi vao ban
			if(countInQueue==0) return;
			// customer* tmp=order;
			// while(findCus(positionX,tmp->name)!=NULL){
			// 	tmp=tmp->next;
			// }
			while(countInDesk!=MAXSIZE && countInQueue!=0){
				string name=headCustomerInQueue->name;
				int energy=headCustomerInQueue->energy;
				remove(headCustomerInQueue,headCustomerInQueue);
				RED(name,energy);
			}
		}
		void PURPLE()
		{
			// cout << "purple"<< endl;
			// hang cho
			// tìm index có energy cao nhất đầu tiên
			// dùng shell sort sắp xếp từ đầu hàng chờ đến index
			// đếm số lần swap (count) sau đó dùng lệnh blue(count%MAXSIZE)
			if (headCustomerInQueue==NULL) return;
			customer* tmp=headCustomerInQueue;
			customer* cusMaxEnergy=headCustomerInQueue;
			int indexMax=0;
			int index=0;
			do{
				if(abs(tmp->energy)>abs(cusMaxEnergy->energy)){
					cusMaxEnergy=tmp;
					indexMax=index;
				}
				else if(abs(tmp->energy)==abs(cusMaxEnergy->energy)&&sttOfCus(tmp->name)>sttOfCus(cusMaxEnergy->name)){
					cusMaxEnergy=tmp;
					indexMax=index;
				}
				tmp=tmp->next;
				index++;
			}
			while(tmp!=headCustomerInQueue);
			int N=shellsort(headCustomerInQueue,indexMax+1);
			BLUE(N%MAXSIZE);
		}
		void REVERSAL()
		{
			// cout << "reversal" << endl;
			// từ vị trí positionX
			// tạo 2 biến 1 biến cùng dấu position 1 biến khác dấu
			// 
			if(positionX==NULL) return;
			customer* sameSign=positionX;
			customer* tmp=positionX->next;
			customer* difSign=positionX;
	
			do{
				if(sameSign->energy* tmp->energy>0&&sameSign->energy*positionX->energy>0){
					swap(sameSign,tmp);
					sameSign=sameSign->prev;
					tmp=tmp->next;
				}
				if(sameSign==tmp) break;
				sameSign=(sameSign->energy*positionX->energy<0)? sameSign->prev:sameSign;
				tmp=(tmp->energy*positionX->energy<0)? tmp->next:tmp;

			}while (sameSign!=tmp&& sameSign->next!=tmp);
			tmp=difSign->next;
			do{
				if(difSign->energy*tmp->energy>0 && difSign->energy*positionX->energy<0){
					swap(difSign,tmp);
					difSign=difSign->prev;
					tmp=tmp->next;
				}
				if(difSign==tmp) break;
				difSign=(difSign->energy*positionX->energy>0)? difSign->prev:difSign;
				tmp=(tmp->energy*positionX->energy>0)? tmp->next:tmp;
			}while(difSign!=tmp &&difSign->next!=tmp);

			
		}
		void UNLIMITED_VOID()
		{
			// cout << "unlimited_void" << endl;
			// tìm dãy con liên tiếp dài nhất (có độ dài tối thiểu là 4)mà có tổng energy bé nhất
			// sau đó in thông tin dãy vừa tìm đc bắt đầu từ phần tử bé nhất "name-energy\n"
			// trường hợp nhiều dãy con thỏa thì lấy dãy cuối cùng tìm đc

			if(countInDesk<4) return;
			int indexBegin=0;
			int sizeMax=0;
			int i=0;
			int sum=0;
			int sumMin=99999;
			int size=0;
			customer* tmp=positionX;
			do{
				sum=0;
				size=0;
				customer*temp=tmp;
				while(size<3){
					sum+=temp->energy;
					temp=temp->next;
					size++;
				}
				do{

					size++;
					sum+=temp->energy;
					if(sum==sumMin&&size>=sizeMax){
						sizeMax=size;
						indexBegin=i;
					}
					if(sum<sumMin){
						sizeMax=size;
						sumMin=sum;
						indexBegin=i;
					}
					temp=temp->next;
					
				}while (temp!=tmp);
				tmp=tmp->next;
				i++;
			}while(tmp!=positionX);
		printSubList(indexBegin,sizeMax);
		}


		void DOMAIN_EXPANSION()
		{
			// cout << "domain_expansion" << endl;
			// tính tổng energy của chú linh và chú thuật sư tổng trị td ai bé hơn sẽ bị đuỏi về
			// và in ra thông tin "name-energy\n" từ kh đến sau đến kh đến đầu tiên
			// tức là đuổi từ queue đén trong bàn
			// sau đó bố trí khách từ hàng đợi vào chô trống
			customer* tmp=order;
			int eThuatSu=0;
			int eOanLinh=0;
			int countThuatSu=0;
			int countOanLinh=0;
			if(order==NULL) return;
			do{
				if(tmp->energy<0){
					eOanLinh+=tmp->energy;
					countOanLinh++;
				}
				else{
					eThuatSu+=tmp->energy;
					countThuatSu++;
				}
				tmp=tmp->next;
			}while(tmp!=order);

			kickCustomer(abs(eThuatSu)>=abs(eOanLinh),countThuatSu,countOanLinh);
			// them kh tu hang doi vao ban
			if(countInQueue==0) return;
			// tmp=order;
			// while(findCus(positionX,tmp->name)!=NULL){
			// 	tmp=tmp->next;
			// }
			while(countInDesk!=MAXSIZE && countInQueue!=0){
				string name=headCustomerInQueue->name;
				int energy=headCustomerInQueue->energy;
				remove(headCustomerInQueue,headCustomerInQueue);
				RED(name,energy);
			}
		}
		void LIGHT(int num)
		{
			// cout << "light " << num << endl;
			if(num==0&&headCustomerInQueue!=NULL){
				customer* tmp=headCustomerInQueue;
				do{
					tmp->print();
					tmp=tmp->next;
				}
				while(tmp!=headCustomerInQueue);
			}
			if(num>0&&positionX!=NULL){
				customer*tmp=positionX;
				do{
					tmp->print();
					tmp=tmp->next;
				}
				while(tmp!=positionX);
			}
			else if(num<0&&positionX!=NULL){
				customer*tmp=positionX;
				do{
					tmp->print();
					tmp=tmp->prev;
				}
				while(tmp!=positionX);
			}
		}
};