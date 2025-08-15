::NAME <- "FRIEDMAN";
::AUTHOR <- "James W. Friedman";

function make_move(history)
{
	if (history.other.len() == 0)
	{
		return EMove.COOPERATION;
	}

	if (history.other.find(EMove.DEFECTION))
	{
		return EMove.DEFECTION;
	}

	return EMove.COOPERATION;
}

// enum EMove {
// 	NONE,
// 	COOPERATION,
// 	DEFECTION
// }

// local history = {
// 	self = [ EMove.COOPERATION /* ... */ ],
// 	other = [ EMove.DEFECTION /* ... */ ]
// };