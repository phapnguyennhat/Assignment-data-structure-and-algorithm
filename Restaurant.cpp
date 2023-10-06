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

	private:
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
			positionX=(head==headCustomerInDesk	&&cus->energy>0) ?cus->next:cus->prev; 
			head=(head==cus)? head->next:head;
			cus->prev->next=cus->next;
			cus->next->prev=cus->prev;
			cus->next=nullptr;
			cus->prev=nullptr;
			delete cus;
		}
		void insert_next(customer* positionX,customer* cus){
			
			cus->prev=positionX;
			cus->next=positionX->next;
			positionX->next->prev=cus;
			positionX->next=cus;
		}
		void insert_pre (customer* positionX,customer* cus){
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
		void kickCustomer(bool isOanLinh)
		{
			// đuổi khách hàng từ vị khách mới đến cho đến kh đến sớm nhất
			if(headCustomerInQueue!=NULL){
				customer*tmp=headCustomerInQueue->prev;
				do{
					bool check=(isOanLinh)? tmp->energy<0:tmp->energy>0;
					if(check){
						cout<<tmp->name<<"-"<<tmp->energy<<"/n";
						tmp=tmp->next;
						remove(headCustomerInQueue,tmp->prev);
					}
					tmp=tmp->prev;
				}
				while(tmp!=headCustomerInQueue->prev);
			}
			if(orderInDesk!=NULL){
				customer*tmp=orderInDesk->prev;
				do{
					bool check=(isOanLinh)? tmp->energy<0:tmp->energy>0;
					if(check){
						cout<<tmp->name<<"-"<<tmp->energy<<"/n";
						tmp=tmp->next;
						remove(orderInDesk,tmp->prev);
						remove(headCustomerInDesk,tmp->prev);
					}
					tmp=tmp->prev;
				}
				while(tmp!=orderInDesk->prev);
			}
		}
		void printSubList(int indexBegin,int size){
			// xuất thông tin chuỗi con từ vị trí indexbegin tính từ positionX
			// với size phần tử
			
			for(int i=0;i<size;i++){
				cout<<customerAt(positionX,i+indexBegin)->name<<"-"<<customerAt(positionX,i+indexBegin)->energy<<"/n";

			}
		
		}
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
					insert_next(orderInDesk->prev,cusOrder);
					if(energy>=positionX->energy){
						// chieu dong ho
						insert_next(positionX,cus);
					}
					else{
						// nguoc chieu dong ho
						insert_pre(positionX,cus);
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
						insert_next(tail,cus);
					}
					countInQueue++;
				}
				else if(countInDesk<MAXSIZE){
					// dua vao ban theo chien thuat 2
					insert_next(orderInDesk->prev,cusOrder);
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
			// tạo 2 biến 1 biến cùng dấu position 1 biến khác dấu
			// 
			customer* sameSign=positionX;
			customer* tmp=positionX->next;
			customer* difSign=positionX;
			do{
				difSign=difSign->prev;
				if (sameSign==difSign){
					// tat ca deu duong hoac tat ca deu am
					difSign=NULL;
					break;
				}
			}
			while(sameSign->energy*difSign->energy>0);
			
			for(int i=0;i<countInDesk/2;i++){
				if(sameSign->energy*tmp->energy>0){
					swap(sameSign,tmp);
					// di chuyen den vi tri cung dau position tiep theo
					do{
						sameSign=sameSign->prev;
						// if(sameSign==tmp) break;
					}
					while(sameSign->energy*positionX->energy<0);

				}

				if(difSign!=NULL&&difSign->energy* tmp->energy>0){
					swap(difSign,tmp);
					// di chuyen den vi tri nguoc dau position tiep theo
					do{
						difSign=difSign->prev;
						// if (difSign==tmp) break;
					}
					while(difSign->energy*positionX->energy>0);

				}
				tmp=tmp->next;
			}
			
		}
		void UNLIMITED_VOID()
		{
			cout << "unlimited_void" << endl;
			// tìm dãy con liên tiếp dài nhất (có độ dài tối thiểu là 4)mà có tổng energy bé nhất
			// sau đó in thông tin dãy vừa tìm đc bắt đầu từ phần tử bé nhất "name-energy/n"
			// trường hợp nhiều dãy con thỏa thì lấy dãy cuối cùng tìm đc

			if(countInDesk<4) return;
			int indexBegin=0;
			int size=0;
			int sizeMax=0;
			int sumMin=INT_MAX;
			int sum=0;
			for(int i=0;i<=countInDesk-4;i++){
				customer*tmp=customerAt(positionX,i);
				size=0;
				sum=0;
				
				while(size<4||(tmp->energy<0&&tmp!=positionX)){
					sum+=tmp->energy;
					size++;
					tmp=tmp->next;
				}
				// if(tmp->next==customerAt(positionX,i)&&sum<=sumMin){
				// 	printSubList(indexBegin,size);
				// 	return;
				// }
				// while(tmp->energy<0&&tmp!=positionX){
				// 	sum+=tmp->energy;
				// 	size++;
				// 	tmp=tmp->next;
				// }
				if(sum==sumMin&&size>=sizeMax){
					sizeMax=size;
					indexBegin=i;
				}
				if(sum<sumMin){
					sizeMax=size;
					sumMin=sum;
					indexBegin=i;
				}
			}
			printSubList(indexBegin,sizeMax);
		}


		void DOMAIN_EXPANSION()
		{
			cout << "domain_expansion" << endl;
			// tính tổng energy của chú linh và chú thuật sư tổng trị td ai bé hơn sẽ bị đuỏi về
			// và in ra thông tin "name-energy/n" từ kh đến sau đến kh đến đầu tiên
			// tức là đuổi từ queue đén trong bàn
			// sau đó bố trí khách từ hàng đợi vào chô trống
			customer* desk=headCustomerInDesk;
			customer* queue=headCustomerInQueue;
			int eThuatSu=0;
			int eOanLinh=0;
			do{
				if(desk->energy<0) eOanLinh+=desk->energy;
				else eThuatSu+=desk->energy;
				desk=desk->next;
			}
			while(desk!=headCustomerInDesk);
			do{
				if(queue->energy<0) eOanLinh+=queue->energy;
				else eThuatSu+=queue->energy;
			}
			while(queue!=headCustomerInQueue);
			kickCustomer(abs(eThuatSu)>=abs(eOanLinh));
			while(countInDesk<MAXSIZE&&headCustomerInQueue!=NULL){
				RED(headCustomerInQueue->name,headCustomerInQueue->energy);
				remove(headCustomerInQueue,headCustomerInQueue);
			}
		}
		void LIGHT(int num)
		{
			cout << "light " << num << endl;
			if(num==0&&headCustomerInQueue!=NULL){
				customer* tmp=headCustomerInQueue;
				do{
					cout<<tmp->name<<"-"<<tmp->energy<<"/n";
					tmp=tmp->next;
				}
				while(tmp!=headCustomerInQueue);
			}
			if(num>0&&headCustomerInDesk!=NULL){
				customer*tmp=positionX;
				do{
					cout<<tmp->name<<"-"<<tmp->energy<<"/n";
					tmp=tmp->next;
				}
				while(tmp!=positionX);
			}
			else if(num<0&&headCustomerInDesk!=NULL){
				customer*tmp=positionX;
				do{
					cout<<tmp->name<<"-"<<tmp->energy<<"/n";
					tmp=tmp->prev;
				}
				while(tmp!=positionX);
			}
		}
};