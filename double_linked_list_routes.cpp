#include<iostream>
using namespace std;

struct RouteNode{
    string StopID;
    RouteNode* next;
    RouteNode* prev;

    RouteNode(string id){
        StopID=id;
        next=nullptr;
        prev=nullptr;
    }
};

class RouteManager{
    RouteNode* head;
    RouteNode* tail;

    public:

        RouteManager(){
            head=nullptr;
            tail=nullptr;
        }

        void addStopToRoute(string stopid){
            RouteNode* newnode=new RouteNode(stopid);

            if(tail==nullptr){
                head=tail=newnode;
            }
            else{
                tail->next=newnode;
                newnode->prev=tail;
                tail=newnode;
            }
        }

        RouteNode* routeFind(string id){
            RouteNode* curr=head;
            while(curr!=nullptr){
                if(curr->StopID==id){
                    return curr;
                }
                curr=curr->next;
            }
            return nullptr;
        }

        void displayRoute(){
            if(head==nullptr){
                cout<<"Route is empty"<<endl;
                return;
            }
            RouteNode* curr=head;
            cout<<"====================Routes=================="<<endl;
            while(curr!=nullptr){
                cout<<curr->StopID;
                if(curr->next!=nullptr){
                    cout<<"->";
                }
                curr=curr->next; 
            }
            cout<<endl;
        }

};

int main(){


    return 0;
}

