::NAME <- "TIT FOR TAT";
::AUTHOR <- "Anatol Rapoport";

function make_move(history)
{
	if (history.other.len() == 0)
	{
		return EMove.COOPERATION;
	}

	return history.other.top();
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