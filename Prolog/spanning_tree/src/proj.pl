/**
    Project Name    : FLP logic project - Kostra grafu (Spanning tree)
    Author          : Timotej Ponek, xponek00
    Academic year   : 2022/2023
*/

:- ensure_loaded(input).
:- use_module(library(ugraphs)).

/**
* lists_to_pairs(+ListWithEdge : list, -Pair : pair).
*
* Converts list [[A],[B]] to pair [A-B]
*
* @param ListWithEdge
* @param Pair
*/
list_to_pair([],[]) :- !.
list_to_pair([[H],[T]], H-T).

/**
* lists_to_pairs(+ListOfEdges : list, -ListOfPairs : pair).
*
* Converts list [[[A],[B]],...] to list of pairs [A-B,...]
*
* @param ListOfEdges
* @param ListOfPairs
*/
lists_to_pairs([],[]) :- !.
lists_to_pairs([L|Ls],[H|T]) :-
    lists_to_pairs(Ls,T), 
    list_to_pair(L,H).

/**
* is_connected(+Vertices : list, +Graph : list, +TraslatedGraph : list, Result : boolean).
*
* Checks whether graph is a connected graph, returns result in Result variable
* (this is maybe not convenient, but I am lazy to reimplement it)
*
* @param Vertices List of verticies that are found in graph
* @param Graph Graph created from input edges
* @param TraslatedGraph Contains same vertices as Graph, but edges are reversed
* @param Result true if graph is connected, false otherwise
*/
is_connected([], _, _, true).
is_connected([H|T], Graph, TransGraph, Result) :- 
    neighbours(H, Graph, NV1),
    neighbours(H, TransGraph, NV2),
    ( length(NV1, 0), length(NV2, 0) -> 
        Result = false ; 
        is_connected(T, Graph, TransGraph, Result) ).

/**
* is_spanningTree(+Vertices : list, +Edges : list).
*
* Succeeds if pairs in Edges form spanning tree
*
* @param Vertices List of verticies that are found in graph
* @param Edges List with pairs to check
*/
is_spanningTree([], []) :- !, false.
is_spanningTree([], _) :- !.
is_spanningTree([H|T], [A-B|KT]) :-
    (H\=A,H\=B), !, search_in_pairs(H, KT), is_spanningTree(T, [A-B|KT]).
is_spanningTree([_|T], L) :- is_spanningTree(T, L).

/**
* search_in_pairs(+Vertex: atom, +Edges : list).
*
* Succeeds if Vertex is found in any pair obtained from Edges
*
* @param Vertex Vertex to search for
* @param Edges List with pairs to search in
*/
search_in_pairs(_, []) :- !, false.
search_in_pairs(H, [A-B|T]) :-
    (H\=A,H\=B) ->
        search_in_pairs(H, T);
    !.

/**
* are_spanningTrees(+Vertices : list, +Graphs : list, -SpanningTree : list).
*
* Outputs all spanning trees which found in Graphs to SpanningTree variable
*
* @param Vertices List of verticies
* @param Graphs List with potential spanning trees
* @param SpanningTree Next spanning tree found in Graphs, or false
*/
are_spanningTrees(_, [], _) :- !, false.
are_spanningTrees(Vertices, [H|T], SpanningTree) :-
    (is_spanningTree(Vertices, H),
    SpanningTree = H);
    are_spanningTrees(Vertices, T, SpanningTree).

/**
* combinations(+N : int, +Input : list, -Combination : list).
*
* Outputs all cobinations of Input with lenght N edges to Combination
* inspired and basicly adopted (this function is basicaly my take how to
* rewrite it and understand magic behind it) from:
* @see https://www.reddit.com/r/prolog/comments/xyvd6w/comment/irj20gt/?utm_source=share&utm_medium=web3x
*
* @param N desired lenght of output combination
* @param Input List with pairs of edges
* @param Combination Combination of edges of lenght N
*/
combinations(N, Input, Combination) :-
    length(Input, Len),
    Len >= N,
    combinations_N(N, Input, Combination).

/**
* combinations_N(+N : int, +Input : list, -Combination : list).
*
* Outputs all cobinations of Input edges to Combination
* inspired and basicly adopted (this function is basicaly my take how to
* rewrite it and understand magic behind it) from:
* @see https://www.reddit.com/r/prolog/comments/xyvd6w/comment/irj20gt/?utm_source=share&utm_medium=web3x
*
* @param N number of elements left to add to output
* @param Input List with pairs of edges
* @param Combination Output combination of edges
*/
combinations_N(0, [], []).
combinations_N(N, [H|T], Combination) :-
    N2 is N - 1,
    (combinations_N(N2, T, Combinations2),
    Combination = [H|Combinations2])
    ;(combinations_N(N, T, Combination)).

/**
* print_solution(+Solution : list).
*
* Outputs single Solution in format desired by assignment
*
* @param Solution
*/
print_solution([]).
print_solution([H|T]) :-
    format("~w ", [H]),
    print_solution(T).

/**
* print_solutions(+Solutions : list).
*
* Outputs Solutions in format desired by assignment
*
* @param Solutions
*/
print_solutions([]).
print_solutions([H|T]) :-
    print_solution(H),
    format("\n"),
    print_solutions(T).

main :-
    prompt(_, ''),
    read_lines(LL),
    split_lines(LL,S),
    lists_to_pairs(S,Edges),
    vertices_edges_to_ugraph(_, Edges, Graph),
	transpose_ugraph(Graph, TransGraph),
    /** deprecated name in new versions */
    % @see https://www.swi-prolog.org/pldoc/doc/_SWI_/library/ugraphs.pl?show=src#transpose_ugraph/2
    %transpose(Graph, TransGraph),
    /** check whether graph is a connected graph*/
    vertices(Graph, Vertices),
    is_connected(Vertices, Graph, TransGraph, IsConnected),
    IsConnected,
    /** find all possible combinations of input edges with length (|Vertices| - 1)
     * and get only those that form spanning trees */
    length(Vertices, VLen),
    N is VLen - 1,
    findall(V, combinations(N, Edges, V), Kom),
    findall(K, are_spanningTrees(Vertices, Kom, K), Kostry),
    print_solutions(Kostry),
    halt.
