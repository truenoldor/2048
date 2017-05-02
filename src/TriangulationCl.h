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

	//векторное произведение
	static int vp( oxygine::Vector2 &a, oxygine::Vector2 &b, oxygine::Vector2 &c)
	{ return (a.x*b.y - a.x*c.y - b.x*a.y + b.x*c.y + c.x*a.y - c.x*b.y ); }

	//скалярное произведение
	static int scp( oxygine::Vector2 &a, oxygine::Vector2 &b, oxygine::Vector2 &c )
	{ return( (c.x - b.x)*(a.x - b.x) + (c.y - b.y)*(a.y - b.y) ); }

	//добавление в список
	void appendtoTail(const T &t){ lp.appendtoTail(t); }

	//удаление списка
	void clear(){ lp.clear(); }

	//количество точек многоугольника
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

//функция определяет принадлежность точки point треугольнику t1_t2_t3
bool Triangulation::isInTriangle( oxygine::Vector2 &point, oxygine::Vector2 &t1, oxygine::Vector2 &t2, oxygine::Vector2 &t3 )
{
	int a, b, c;
	a = vp( point, t1, t2 );
	b = vp( point, t2, t3 );
	c = vp( point, t3, t1 );
	//Если все три двойки векторов однонаправленные, то point внутри треугольника
	return ( a < 0 && b < 0 && c < 0 || a > 0 && b > 0 && c > 0 );	
}

//функция определяет направление обхода многоугольника
void Triangulation::determineDirection()
{
	int maxx = lp.head->p.x;
	Node *np, *mp = lp.tail;
	oxygine::Vector2 p1, p2, p3, px;
	
	//находим точку с максимальным значением по Х
	for( np = lp.head; np != lp.tail; np = np->next )
		if( maxx < (np->next->p.x ))
		{
			maxx = np->next->p.x;
			mp = np;
		}
	p1 = mp->p;
	p2 = mp->next->p;
	p3 = mp->next->next->p;
	//с помощью векторного произведения определяем 
	//направление тройки векторов
	if( vp( p1, p2, p3 ) > 0 )	direction = 1; // по часовой стрелке
	else direction = -1;//против часовой стрелки
}

//функция триангуляции
//vector < Polygon_ > &vn - массив с фигурами
//vn[0] - будет содержать (после вызова этой функции) массив вершин исходного многоугольника
//vn[1] ... vn[vn.size()-1] - массив вершин треугольников
void Triangulation::triangulation( vector < Polygon_ > &vn )
{
	int i, t;
	bool newTriangle;
	//первые 3 вершины
	Node *pn1 = lp.head, *pn2 = lp.head->next, *pn3 = lp.head->next->next, *it1;

	Polygon_ pol;
	pol.p = new oxygine::Vector2[lp.size];
	pol.size = lp.size;
	//определяем направление обхода многоугольника
	determineDirection();

	//заносим в vector массив вершин многоугольника
	for( i = 0, pn1 = lp.head; i < lp.size; pn1 = pn1->next, i++ )
		pol.p[i] = pn1->p;
	vn.push_back(pol);

	

	

	vector< TestCycle > tests;

	//пока не останется 3 точки
	while( lp.size != 3 )
	{
		//если вектора образуют правую тройку
		if( vp( pn2->p, pn1->p, pn3->p ) * direction < 0 )
		{
			newTriangle = true;

			//проверка не попала ли вершина в отсекаемый треугольник
			for( it1 = pn3->next; it1 != pn1; it1 = it1->next )
				if( isInTriangle(it1->p, pn1->p, pn2->p, pn3->p) )
				{
					newTriangle = false; //вершина попала в отсекаемый треугольник
					pn1 = pn1->next; //переходим к следующей вершине
					break;
				}
			if( newTriangle == true )//вершина не попала в отсекаемый треугольник
			{
				//заносим в vector новый треугольник
				pol.p = new oxygine::Vector2[3];
				pol.size = 3;
				pol.p[0] = pn1->p;	
				pol.p[1] = pn2->p;	
				pol.p[2] = pn3->p;
				vn.push_back(pol);
				//удаляем вершину из списка
				lp.delete_(pn2->key);
			}
			//сдвигаем 2ю и 3ю вершины
			pn2 = pn3;	pn3 = pn3->next;
		}
		else //вектора не образуют правую тройку,  сдвигаем 3 вершины
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
	//заносим в vector оставшийся треугольник
	pol.p = new oxygine::Vector2[3];
	pol.size = 3;
	pol.p[0] = lp.head->p; 
	pol.p[1] = lp.head->next->p; 
	pol.p[2] = lp.head->next->next->p;
	vn.push_back(pol);
}

#endif