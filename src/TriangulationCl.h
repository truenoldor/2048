#ifndef TRLIB_h
#define TRLIB_h

#define M 40

using namespace std;

class Triangulation
{
public:
	Triangulation(){;};
	Triangulation( SList &listPoints ){ lp = listPoints; };

	void triangulation(vector <Polygon_> &vn);
	static bool isInTriangle( oxygine::Vector2 &point, oxygine::Vector2 &t1, oxygine::Vector2 &t2, oxygine::Vector2 &t3 );

	//��������� ������������
	static int vp( oxygine::Vector2 &a, oxygine::Vector2 &b, oxygine::Vector2 &c)
	{ return (a.x*b.y - a.x*c.y - b.x*a.y + b.x*c.y + c.x*a.y - c.x*b.y ); }

	//��������� ������������
	static int scp( oxygine::Vector2 &a, oxygine::Vector2 &b, oxygine::Vector2 &c )
	{ return( (c.x - b.x)*(a.x - b.x) + (c.y - b.y)*(a.y - b.y) ); }

	//���������� � ������
	void appendtoTail(const T &t){ lp.appendtoTail(t); }

	//�������� ������
	void clear(){ lp.clear(); }

	//���������� ����� ��������������
	int size(){ return lp.size; }

private:
	SList lp;
	int direction;

	struct TestCycle
	{
		Node *pntest1;
		Node *pntest2;
		Node *pntest3;
		TestCycle()
		{
			pntest1 = NULL;
			pntest2 = NULL;
			pntest3 = NULL;
		}
		TestCycle( Node * p1, Node * p2, Node * p3 )
		{
			pntest1 = p1;
			pntest2 = p2;
			pntest3 = p3;
		}
		bool operator==( const TestCycle &src )
		{
			return src.pntest1 == pntest1 && src.pntest2 == pntest2 && src.pntest3 == pntest3;
		}
	};

	void determineDirection();
};

//������� ���������� �������������� ����� point ������������ t1_t2_t3
bool Triangulation::isInTriangle( oxygine::Vector2 &point, oxygine::Vector2 &t1, oxygine::Vector2 &t2, oxygine::Vector2 &t3 )
{
	int a, b, c;
	a = vp( point, t1, t2 );
	b = vp( point, t2, t3 );
	c = vp( point, t3, t1 );
	//���� ��� ��� ������ �������� ����������������, �� point ������ ������������
	return ( a < 0 && b < 0 && c < 0 || a > 0 && b > 0 && c > 0 );	
}

//������� ���������� ����������� ������ ��������������
void Triangulation::determineDirection()
{
	int maxx = lp.head->p.x;
	Node *np, *mp = lp.tail;
	oxygine::Vector2 p1, p2, p3, px;
	
	//������� ����� � ������������ ��������� �� �
	for( np = lp.head; np != lp.tail; np = np->next )
		if( maxx < (np->next->p.x ))
		{
			maxx = np->next->p.x;
			mp = np;
		}
	p1 = mp->p;
	p2 = mp->next->p;
	p3 = mp->next->next->p;
	//� ������� ���������� ������������ ���������� 
	//����������� ������ ��������
	if( vp( p1, p2, p3 ) > 0 )	direction = 1; // �� ������� �������
	else direction = -1;//������ ������� �������
}

//������� ������������
//vector < Polygon_ > &vn - ������ � ��������
//vn[0] - ����� ��������� (����� ������ ���� �������) ������ ������ ��������� ��������������
//vn[1] ... vn[vn.size()-1] - ������ ������ �������������
void Triangulation::triangulation( vector < Polygon_ > &vn )
{
	int i, t;
	bool newTriangle;
	//������ 3 �������
	Node *pn1 = lp.head, *pn2 = lp.head->next, *pn3 = lp.head->next->next, *it1;

	Polygon_ pol;
	pol.p = new oxygine::Vector2[lp.size];
	pol.size = lp.size;
	//���������� ����������� ������ ��������������
	determineDirection();

	//������� � vector ������ ������ ��������������
	for( i = 0, pn1 = lp.head; i < lp.size; pn1 = pn1->next, i++ )
		pol.p[i] = pn1->p;
	vn.push_back(pol);

	

	

	vector< TestCycle > tests;

	//���� �� ��������� 3 �����
	while( lp.size != 3 )
	{
		//���� ������� �������� ������ ������
		if( vp( pn2->p, pn1->p, pn3->p ) * direction < 0 )
		{
			newTriangle = true;

			//�������� �� ������ �� ������� � ���������� �����������
			for( it1 = pn3->next; it1 != pn1; it1 = it1->next )
				if( isInTriangle(it1->p, pn1->p, pn2->p, pn3->p) )
				{
					newTriangle = false; //������� ������ � ���������� �����������
					pn1 = pn1->next; //��������� � ��������� �������
					break;
				}
			if( newTriangle == true )//������� �� ������ � ���������� �����������
			{
				//������� � vector ����� �����������
				pol.p = new oxygine::Vector2[3];
				pol.size = 3;
				pol.p[0] = pn1->p;	
				pol.p[1] = pn2->p;	
				pol.p[2] = pn3->p;
				vn.push_back(pol);
				//������� ������� �� ������
				lp.delete_(pn2->key);
			}
			//�������� 2� � 3� �������
			pn2 = pn3;	pn3 = pn3->next;
		}
		else //������� �� �������� ������ ������,  �������� 3 �������
		{
			pn1 = pn2;	pn2 = pn3;	pn3 = pn3->next;
			TestCycle ts( pn1, pn2, pn3 );
			bool isFind = false;
			for ( int i = 0; i < ( int )tests.size(); i++ )
			{
				if ( tests[ i ] == ts )
				{
					isFind = true;
					break;
				}
			}

			if ( isFind )
				break;

			tests.push_back( ts );
		}
	}
	//������� � vector ���������� �����������
	pol.p = new oxygine::Vector2[3];
	pol.size = 3;
	pol.p[0] = lp.head->p; 
	pol.p[1] = lp.head->next->p; 
	pol.p[2] = lp.head->next->next->p;
	vn.push_back(pol);
}

#endif