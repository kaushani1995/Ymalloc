#include<stdio.h>
#include<stdlib.h>

struct block
{
	void* start;
	void* end;
	struct block* next;
};

struct block* uastr=NULL;
struct block* astr=NULL;

int count =0;

void* yalloc(size_t tsize);
int setunalloc(void* p, size_t size);
void* ymalloc(size_t size);
void* check(size_t size);
int mlistequal(struct block* prept, struct block* pt);
int mlistlarger(struct block* pt, size_t size);
int yfree(void* p);
int merge(struct block* pt);
int delete(struct block* pt);


int setunalloc(void* p, size_t size)
{
//	printf("\nsetunalloc");
	struct block* a = uastr;
	a->start=p;
	a->end=p+size-1;
	a->next=NULL;
	count++;
	return 0;
}


void* yalloc(size_t tsize)
{	
//	printf("\nyalloc");
	void *p;
	p = malloc(tsize+(101*sizeof(struct block)));
	uastr=(struct block*)p;
	p = p+(101*sizeof(struct block));
	setunalloc(p,tsize);
	return p;
}



void* ymalloc(size_t size)
{
//	printf("\nymalloc");
	void* p;	
	if(count==101)
        {
                printf("\nAllocation not allowed. Number of allocations exceeded!\n");
                exit(0);
        }
		
	p=check(size);
	
	if(p==NULL)
	{	
		printf("\nRequested space unavailable!!\n");	
		return NULL;
	}
	else
		return p;
}

int mlistequal(struct block* prept, struct block* pt)
{
//	printf("\nmlistequal");
	struct block* p=astr;
	prept->next=pt->next;
	while(p->next!=NULL)
		p=p->next;
	p->next=pt;
	pt->next=NULL;
}

void* check(size_t size)
{
//	printf("\ncheck");
	struct block* pt=uastr;
	struct block* prept=uastr;
	void* p;
	int flag=0;

	while(pt!=NULL)
	{
//		printf("\nhere");
		if(((pt->end)-(pt->start)+1)==size) 
		{
			while(prept->next!=pt)
				prept=prept->next;
			p=pt->start;
			mlistequal(prept, pt);
			flag=1;
			goto label;
		}
		pt=pt->next;
	}

	pt=uastr;
	while(pt!=NULL)
	{
//		printf("\nhere2");
		if(((pt->end)-(pt->start)+1)>size)
		{
			p=pt->start;
			mlistlarger(pt,size);
			flag=1;
			goto label;
		}
		pt=pt->next;	
	}

	label:
	if(flag==1)
	{
//		printf("\nreturn from check");
		count++;
		return p;
	}
	else 
		return NULL;	
}

int mlistlarger(struct block* pt, size_t size)
{
//	printf("\nmlistlarger");
	struct block* temp = uastr+(count*(sizeof(struct block)));
	struct block* a=astr;
	if(((pt->end)-(pt->start)+1)>size)
	{
//		printf("\npt->start=%p",pt->start);
		if(count==1)
		{
//			printf("\nFirst astr");
			astr=temp;
			temp->start=pt->start;
       		        temp->end=(temp->start+size-1);
       	       		temp->next=NULL;
			pt->start=pt->start+size;
		}
		else
		{
//			printf("\nremaining astr");
			while(a->next!=NULL)
				a=a->next;
			a->next=temp;
			temp->start=pt->start;
			temp->end=(temp->start+size-1);
			temp->next=NULL;
			pt->start=pt->start+size;
		}
//		printf("\nnew pt->start=%p, pt->end=%p",pt->start,pt->end);
	}
	return 0;
}

int delete(struct block* pt)
{
//	printf("\ndelete");
	struct block* prept=uastr;
	while(prept->next!=pt)
		prept=prept->next;
	prept->next=pt->next;
}

int merge(struct block* pt)
{
//	printf("\nmerge");
	struct block* p=uastr;
	while(p!=NULL)
	{
		if((p->end+1)==pt->start)
		{
			p->end=pt->end;
			delete(pt);
			pt=p;
			break;
		}
		p=p->next;
	}	
	
	p=uastr;
	while(p!=NULL)
	{
		if((p->start-1)==pt->end)
		{
			p->start=pt->start;
			delete(pt);
			break;
		}
		p=p->next;
	}
	return 0;
}

int yfree(void* p)
{
//	printf("\nyfree");
	struct block* pt=astr;
	struct block* prept=astr;
	struct block* upt=uastr;
	int flag=0;
	while(pt!=NULL)
	{
		if(pt->start==p)
		{
			flag=1;
			break;
		}
		else
			pt=pt->next;
	}
	
	if(flag==1)
	{
		while(prept->next!=pt)
			prept=prept->next;
		prept->next=pt->next;
		pt->next=NULL;
		while(upt->next!=NULL)
			upt=upt->next;
		upt->next=pt;
		merge(pt);
		return 0;
	}
	else
	{
		printf("\nInvalid address to be freed!");
		return 0;
	}
}


int main()
{
	//TEST CODE
	void *p1, *p2, *p4;
	int *p5, *p3;
	p1=yalloc(100);
	printf("\n%p",p1);
	p5=(int*)ymalloc(sizeof(int));
	*p5=9;
	printf("\n%p, %d",p5, *p5);
	p2=ymalloc(80);
	printf("\n%p",p2);
	yfree((void*)p2);
	p3=(int*)ymalloc(100);
	yfree((void*)p3);
	p4=ymalloc(90);
	printf("\n%p\n",p4);
	yfree((void*)p4);

	return 0;
}
