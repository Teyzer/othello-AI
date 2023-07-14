(* Fichier à compiler en invoquant le compilateur de la façon suivante :

	ocamlfind ocamlc -package yojson,unix ...
	ou
	ocamlfind ocamlopt -package yojson,unix ...

	suivi des options habituelles de compilation.

	ocamlfind est un outil qui permet de compléter la ligne de commande du
	compilateur en ajoutant les directives nécessaires pour trouver les
	bibliothèques données par le paramètre -package.
*)
   let connect_to_server hostname port =
	let open Unix in
	let addrinfos = getaddrinfo hostname "" [AI_FAMILY(PF_INET); AI_SOCKTYPE(SOCK_STREAM)] in
	let tryconnect ai =
		let sock = socket ai.ai_family ai.ai_socktype 0 in
		connect sock ai.ai_addr;
		sock
	in let sock = tryconnect (List.hd addrinfos) in
	(in_channel_of_descr sock, out_channel_of_descr)

type server_board = {
	data : string;
	width: int;
	height: int;
	your_color: char;
}

type server_message =
	| Joined of char
	| Your_turn of server_board
	| Move_done of server_board
	| Wrong_move of server_board
	| Game_over of server_board * char

let get_color mappings =
	(Yojson.Basic.Util.to_string (List.assoc "your_color" mappings)).[0]

let get_board json =
	let mappings = Yojson.Basic.Util.to_assoc json in
	{
		data = Yojson.Basic.Util.to_string (List.assoc "board" mappings);
		width = Yojson.Basic.Util.to_int (List.assoc "width" mappings);
		height = Yojson.Basic.Util.to_int (List.assoc "height" mappings);
		your_color = get_color mappings;
	}

let get_message server_in_chan =
	let json = Yojson.Basic.from_string (input_line server_in_chan) in
	let mappings = Yojson.Basic.Util.to_assoc json in
	match Yojson.Basic.Util.to_string (List.assoc "status" mappings) with
	| "joined" -> Joined(get_color mappings)
	| "your_turn" -> Your_turn(get_board json)
	| "move_done" -> Move_done(get_board json)
	| "wrong_move" -> Wrong_move(get_board json)
	| "game_over" -> Game_over(get_board json, (Yojson.Basic.Util.to_string (List.assoc "winner" mappings)).[0])
	| _ -> invalid_arg "unexpected server status"
