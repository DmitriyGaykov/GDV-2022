#include "stdafx.h"
#include "Error.h"

namespace Error
{
	// ñåğèè îøèáîê:	0 - 99 - ñèñòåìíûå îøèáêè
	//					100 - 109 - îøèáêè ïàğàìåòğû
	//					110 - 119 - îøèáêè îòêğûòèÿ è ÷òåíèÿ ôàéëîâ

	ERROR errors[ERROR_MAX_ENTRY] = // òàáëèöà îøèáîê
	{
		ERROR_ENTRY(0, "Íåäîïóñòèì êîä îøèáêè"),
		ERROR_ENTRY(1, "Ñèñòåìíûé ñáîé"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Ïàğàìåòğ -in äîëæåí áûòü çàäàí"),
		ERROR_ENTRY_NODEF(101),ERROR_ENTRY_NODEF(102),ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Ïğåâûøåíà äëèíà âõîäíîãî ïàğàìåòğà"),
		ERROR_ENTRY_NODEF(105),ERROR_ENTRY_NODEF(106),ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Îøèáêà ïğè îòêğûòèè ôàéëà ñ èñõîäíûì êîäîì (-in)"),
		ERROR_ENTRY(111, "Íåäîïóñòèìûé ñèìâîë â èñõîäíîì ôàéëå (-in)"),
		ERROR_ENTRY(112, "Îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"),
		ERROR_ENTRY(113, "Îøèáêà ïğè ñîçäàíèè òàáëèöû ëåêñåì. Ïğåâûøåíà åìêîñòü òàáëèöû ëåêñåì"),
		ERROR_ENTRY(114, "Îøèáêà ïğè äîáàâëåíèè ëåêñåìû â òàáëèöó. Òàáëèöà ëåêñåì çàïîëíåíà"),
		ERROR_ENTRY(115, "Îøèáêà ïğè ïîëó÷åíèè ëåêñåìû èç òàáëèöû. Íåäîïóñòèìûé íîìåğ ëåêñåìû"),
		ERROR_ENTRY(116, "Îøèáêà ïğè ñîçäàíèè òàáëèöû èäåíòèôèêàòîğîâ. Ïğåâûøåíà åìêîñòü òàáëèöû èäåíòèôèêàòîğîâ"),
		ERROR_ENTRY(117, "Îøèáêà ïğè äîáàâëåíèè èäåíòèôèêàòîğà â òàáëèöó. Òàáëèöà èäåíòèôèêàòîğîâ çàïîëíåíà"),
		ERROR_ENTRY(118, "Îøèáêà ïğè ïîëó÷åíèè çàïèñè èç òàáëèöû èäåíòèôèêàòîğîâ. Íåäîïóñòèìûé íîìåğ èäåíòèôèêàòîğà"),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120),ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160),ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF100(300),ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Íåò çàêğûâàşùåé êàâû÷êè"),
		ERROR_ENTRY(601, "Íàéäåíî íåîïîçíàííîå ñëîâî"),
		ERROR_ENTRY(602, "Èñïîëüçîâàííà íåèíèöèàëèçèğîâàííàÿ ïåğåìåííàÿ"),
		ERROR_ENTRY(603, "Íåêîğğåêòíîå îáúÿâëåíèå ôóíêöèè"),
		ERROR_ENTRY(604, "Ïğè èíèöèàëèçàöèè ôóíêöèè íå õâàòàåò çàêğûâàşùåé ñêîáêè ')'"),
		ERROR_ENTRY(605, "Ïğè èíèöèàëèçàöèè ôóíêöèè íå óêàçàí òèï âîçâğàùàåìîãî çíà÷åíèÿ"),
		ERROR_ENTRY(606, "Îøèáêà. Ïîâòîğíàÿ èíèöèàëèçàöèÿ ïåğåìåííîé!"),
		ERROR_ENTRY(607, "Íåîïğåäåëåííàÿ îøèáêà!"),
		ERROR_ENTRY(608, "Íåîïğåäåëåííàÿ îøèáêà!"),
		ERROR_ENTRY(609, "Íåîïğåäåëåííàÿ îøèáêà!"),
		ERROR_ENTRY_NODEF10(610),ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),ERROR_ENTRY_NODEF10(640),ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660),ERROR_ENTRY_NODEF10(670),ERROR_ENTRY_NODEF10(680),ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) // ïîëó÷åíèå îøèáêè ïî åå êîäó
	{
		if (!(id > 0 && id < ERROR_MAX_ENTRY))
		{
			ERROR e = errors[0]; // íåäîïóñòèìûé êîä îøèáêè
			e.inext.col = e.inext.line = -1; // íåäîïóñòèìûå êîîğäèíàòû îøèáêè
			return e;
		}
		else
		{
			ERROR e = errors[id]; // îøèáêà ïî êîäó
			e.inext.col = e.inext.line = -1; // íåäîïóñòèìûå êîîğäèíàòû îøèáêè
			return e;
		}
	}

	ERROR geterrorin(int id, int line = -1, int col = -1) // âîçâğàùåíèå îøèáêè ïî êîäó, ïëşñ âûâîä ñòğîêó è ìåñòî
	{
		if (!(id > 0 && id < ERROR_MAX_ENTRY))
		{
			ERROR e = errors[0];
			return e;
		}
		else
		{
			ERROR e = errors[id];
			e.inext = { short(line), short(col) };
			return e;
		}
	}
}