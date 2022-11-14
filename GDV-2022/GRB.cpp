#include "GRB.h"

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
#define GRB_ERROR_SERIES 650

#define IFBODY 'C'
#define INIT 'D'
#define TYPES_VALUES 'G'
#define IF 'J'
	
	GRB::Greibach greibach(
		NS('S'),
		TS('$'),
		12, // edit
		
		
		Rule(NS('S'), GRB_ERROR_SERIES + 0, // глобальное пространство
			10,

			Rule::Chain(7, TS('i'), TS('s'), TS('t'), TS('='), NS('E'), TS(';'), NS('S')),
			Rule::Chain(6, TS('i'), TS('s'), TS('t'), TS('='), NS('E'), TS(';')),
			
			Rule::Chain(5, TS('i'), TS('s'), TS('t'), TS(';'), NS('S')),
			Rule::Chain(4, TS('i'), TS('s'), TS('t'), TS(';')),
			
			Rule::Chain(5, TS('i'), TS('='), TS('E'), TS(';'), NS('S')),
			Rule::Chain(4, TS('i'), TS('='), TS('E'), TS(';')),

			Rule::Chain(10, TS('i'), TS('s'), TS('f'), NS('P'), TS('s'), TS('t'), TS('{'), NS('B'), TS('}'), NS('S')),
			Rule::Chain(9, TS('i'), TS('s'), TS('f'), NS('P'), TS('s'), TS('t'), TS('{'), NS('B'), TS('}')),

			Rule::Chain(5, TS('m'), TS('{'), NS('B'), TS('}'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('B'), TS('}'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 1, // выражение
			1,
			Rule::Chain(1, NS('G'))
		),

		Rule(NS('P'), GRB_ERROR_SERIES + 2, // параметры
			2,
			Rule::Chain(2, TS('('), TS(')')),
			Rule::Chain(3, TS('('), NS('I'), TS(')'))
		),

		Rule(NS('I'), GRB_ERROR_SERIES + 3, // инициализация параметров
			2,
			Rule::Chain(3, TS('i'), TS('s'), TS('t')),
			Rule::Chain(5, TS('i'), TS('s'), TS('t'), TS(','), NS('I'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 4, // вызов функции
			2,
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('A'), TS(')'))
		),

		Rule(NS('A'), GRB_ERROR_SERIES + 5, // параметры вызываемой функции
			6,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, NS('F')),

			Rule::Chain(2, NS('E'), NS('A')),
			Rule::Chain(2, NS('E'), NS('A')),
			Rule::Chain(2, NS('E'), NS('A'))
		),

		Rule(NS('B'), GRB_ERROR_SERIES + 6, // тело функции
			9,
			Rule::Chain(3, NS(INIT), TS(';'), NS('B')),
			Rule::Chain(3, NS(INIT), TS(';'), NS('R')),
			
			Rule::Chain(5, NS(INIT), TS('='), NS('E'), TS(';'), NS('B')),
			Rule::Chain(5, NS(INIT), TS('='), NS('E'), TS(';'), NS('R')),
			
			Rule::Chain(2, NS(IF), NS('B')),
			Rule::Chain(2, NS(IF), NS('R')),

			Rule::Chain(4, TS('{'), NS('B'), TS('}'), NS('B')),
			Rule::Chain(4, TS('{'), NS('B'), TS('}'), NS('R')),

			Rule::Chain(1, NS('R'))
		),

		Rule(NS(INIT), GRB_ERROR_SERIES + 7, // инициализация переменных
			1,
			Rule::Chain(3, TS('i'), TS('s'), TS('t'))
			),
		
		Rule(NS(IF), GRB_ERROR_SERIES + 8, // условие
			1,
			Rule::Chain(5, NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), TS('?'), NS(IFBODY))
			),

		Rule(NS('R'), GRB_ERROR_SERIES + 9, // return
			1,
			Rule::Chain(3, TS('R'), NS(TYPES_VALUES), TS(';'))
		),
			
		Rule(NS(TYPES_VALUES), GRB_ERROR_SERIES + 10, // типы значений
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, NS('F')),
			Rule::Chain(1, NS('E'))
		),
				
		Rule(NS(IFBODY), GRB_ERROR_SERIES + 11, // тело if
			3,
			Rule::Chain(4, TS('T'), TS('{'), NS('B'), TS('}')),
			Rule::Chain(8, TS('T'), TS('{'), NS('B'), TS('}'), TS('L'), TS('{'), NS('B'), TS('}')),
			Rule::Chain(4, TS('L'), TS('{'), NS('B'), TS('}'))
		)
	);
	
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	///I?aano. oaii?ee (i?aaay noi?iia i?aaeea)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
		{
			nt[i] = (GRBALPHABET)p[i];
		}
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)	///I?aaeei a a?aiiaoeea A?aeaao
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};
	Greibach getGreibach()	///Iieo?eou a?aiiaoeeo
	{
		return greibach;
	};
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)	///Iieo?eou ? i?aaeea eee -1
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
		{
			k++;
		}
		if (k < size) prule = rules[rc = k];
		return rc;
	};
	Rule Greibach::getRule(short n)	///Iieo?eou i?aaeei ii iiia?o
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};
	char* Rule::getCRule(char* b, short nchain)	///iieo?eou i?aaeei a aeaa N->oaii?ea
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) ///Aa?ioou ? neaa. ca j oaii?ee eee -1
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
		{
			++j;
		}
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};
	char* Rule::Chain::getCChain(char* b)	///iieo?eou oaii?eo a neia. aeaa
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
}