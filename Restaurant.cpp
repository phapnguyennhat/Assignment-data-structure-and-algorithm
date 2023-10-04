#include "main.h"

extern int MAXSIZE;

class imp_res : public Restaurant
{
	private:
		customer* headCustomerInDesk;
		customer* headCustomerInQueue;
		customer* positionX;
		customer* orderInDesk;
		int countInDesk;
		int countInQueue;
	public:	
		imp_res() {
			headCustomerInDesk=NULL;
			headCustomerInQueue=NULL;
			orderInDesk=NULL;
			positionX=NULL;
			countInDesk=0;
			countInQueue=0;
		}
		~imp_res(){
			while(headCustomerInDesk!=NULL){
				remove(headCustomerInDesk,headCustomerInDesk);
			}
			while(headCustomerInQueue!=NULL){
				remove(headCustomerInQueue,headCustomerInQueue);
			}
			while(orderInDesk!=NULL){
				remove(orderInDesk,orderInDesk);
			}
		}
		void remove(customer *& head,customer* cus){
			// xóa phần tử cus trong danh sach lien ket (inDesk,queue,order)
			if(head->next==head && head==cus){
				head->next=nullptr;
				head->prev=nullptr;
				delete head;
				countInDesk=(head==headCustomerInDesk)? 0:countInDesk;
				countInQueue=(head==headCustomerInQueue)? 0:countInQueue;
				head=NULL;
				return;
			}
			countInDesk=(head==headCustomerInDesk)? countInDesk-1:countInDesk;
			countInQueue=(head==headCustomerInQueue)? countInQueue-1:countInQueue;
			head=(head==cus)? head->next:head;
			cus->prev->next=cus->next;
			cus->next->prev=cus->prev;
			cus->next=nullptr;
			cus->prev=nullptr;
			delete cus;
		}
		void push_next(customer* positionX,customer* cus){
			cus->prev=positionX;
			cus->next=positionX->next;
			positionX->next->prev=cus;
			positionX->next=cus;
		}
		void push_pre (customer* positionX,customer* cus){
			cus->next=positionX;
			cus->prev=positionX->prev;
			positionX->prev->next=cus;
			positionX->prev=cus;
		}
		customer* customerAt(customer* head,int index){

			for(int i=0;i<index;i++){
				head=head->next;
			}
			return head;
		}
		void swap(customer* cus1,customer* cus2){
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
				for(int j=i;j>=incr&& abs(customerAt(head,j)->energy)>abs(customerAt(head,j-incr)->energy);j-=incr){
					// swap
					res++;
					swap(customerAt(head,j),customerAt(head,j-incr));
				}
			}
			return res;
		}
		int shellsort(customer* head,int size){
			int res=0;
			for(int i=size/2;i>2;i/=2){
				for(int j=0;j<i;j++){
					res+=inssort2(customerAt(head,j),size-j,i);
				}
			}
			res+=inssort2(head,size,1);
			return res;
		}
		void RED(string name, int energy)
		{
			// reject energy equal 0
			if(energy==0) return;
			// thiên thượng điện hạ, duy ngã độc tôn
			// reject customer whose name is on the desk or the queue
			if(headCustomerInDesk!=NULL){
				customer*tmp=headCustomerInDesk;
				do{
					if(tmp->name==name) return;
					tmp=tmp->next;
				}
				while(tmp!=headCustomerInDesk);
			}
			if(headCustomerInQueue!=NULL){
		
				customer*tmp=headCustomerInQueue;
				do{
					if(tmp->name==name) return;
					tmp=tmp->next;
				}
				while(tmp!=headCustomerInQueue);
			}

			cout<<name<<" "<<energy<<endl;
			customer*cus=new customer(name,energy,nullptr,nullptr);
			customer*cusOrder=new customer(name ,energy,nullptr,nullptr);
			// đưa khách vào bàn hoặc vào hàng đợi
			if(countInDesk<MAXSIZE/2){
				if(headCustomerInDesk==NULL){
					// the first customer
					headCustomerInDesk=cus;
					cus->next=cus;
					cus->prev=cus;
					orderInDesk=cusOrder;
					cusOrder->next=cusOrder;
					cusOrder->prev=cusOrder;
				}
				else{
					// chen khach vao ban
					push_next(orderInDesk->prev,cusOrder);
					if(energy>=positionX->energy){
						// chieu dong ho
						push_next(positionX,cus);
					}
					else{
						// nguoc chieu dong ho
						push_pre(positionX,cus);
					}
				}
				positionX=cus;
				countInDesk++;
			}
			else{
				if(countInDesk==MAXSIZE && countInQueue<MAXSIZE){
					// push tail queue
					if(headCustomerInQueue==NULL){
						headCustomerInQueue=cus;
						cus->next=cus;
						cus->prev=cus;
						
					}
					else{
						customer* tail=headCustomerInQueue->prev;
						push_next(tail,cus);
					}
					countInQueue++;
				}
				else if(countInDesk<MAXSIZE){
					// dua vao ban theo chien thuat 2
					push_next(orderInDesk->prev,cusOrder);
					int sub=0;
					customer* RES=headCustomerInDesk;
					customer* tmp=headCustomerInDesk;
					do
					{
						if (abs(energy - tmp->energy) > abs(sub))
						{
							sub = energy - tmp->energy;
							RES = tmp;
						}
						tmp = tmp->next;
					}
					while(tmp!=headCustomerInDesk);

					if(sub<0){
						push_pre(RES,cus);
					}
					else{
						push_next(RES,cus);
					}
					positionX=cus;
					countInDesk++;
				}
			}
		}
		void BLUE(int num)
		{
			cout << "blue "<< num << endl;

			if(num>=countInDesk){
				delete headCustomerInDesk;
				countInDesk=0;
				delete orderInDesk;
				headCustomerInDesk=NULL;
				orderInDesk=NULL;
			}
			else{
				while(num!=0){
					customer* tmp=headCustomerInDesk;
					do{
						if(tmp->name==orderInDesk->name){
							// popfront orderindex
							remove(orderInDesk,orderInDesk);
							break;

						}
						tmp=tmp->next;
					}
					while(tmp!=headCustomerInDesk);
					
					remove(headCustomerInDesk,tmp);
					num--;
				}
			}

			while(countInDesk<MAXSIZE && headCustomerInQueue!=NULL){
				RED(headCustomerInQueue->name,headCustomerInQueue->energy);
				remove(headCustomerInQueue,headCustomerInQueue);
			}
			
		}
		void PURPLE()
		{
			cout << "purple"<< endl;
			// hang cho
			// tìm index có energy cao nhất đầu tiên
			// dùng shell sort sắp xếp từ đầu hàng chờ đến index
			// đếm số lần swap (count) sau đó dùng lệnh blue(count%MAXSIZE)
			customer* tmp=headCustomerInQueue;
			int absMaxEnergy=abs(headCustomerInQueue->energy);
			int indexMax=0;
			int index=0;
			do{
				if(abs(tmp->energy)>abs(absMaxEnergy)){
					absMaxEnergy=abs(tmp->energy);
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
			cout << "reversal" << endl;
			// từ vị trí positionX
		}
		void UNLIMITED_VOID()
		{
			cout << "unlimited_void" << endl;
		}
		void DOMAIN_EXPANSION()
		{
			cout << "domain_expansion" << endl;
		}
		void LIGHT(int num)
		{
			cout << "light " << num << endl;
		}
};