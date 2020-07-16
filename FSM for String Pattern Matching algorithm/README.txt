## FSM for String Pattern Matching algorithm 
### Background:
String pattern matching algorithms are very useful for several purposes, like simple search for a pattern in a text or looking for attacks with predefined signatures.
We will implement a dictionary-matching algorithm that locates elements of a finite set of strings (the "dictionary") within an input text. It matches all patterns "at once", so the complexity of the algorithm is linear in the length of the patterns plus the length of the searched text plus the number of output matches.
Since all matches are found, there can be a quadratic number of matches if every substring matches (e.g. dictionary = a, aa, aaa, aaaa and input string is aaaa).


The algorithm matches multiple patterns simultaneously, by first constructing a Deterministic Finite Automaton (DFA) representing the patterns set, and then, with this DFA on its disposal, processing the text in a single pass.


Specifically, the DFA construction is done in two phases. First, the algorithm builds a DFA of the patterns set: All the patterns are added from the root as chains, where each state corresponds to one symbol. When patterns share a common prefix, they also share the corresponding set of states in the DFA.
The edges of the first phase are called forward transitions.
The edges of the second phase are called failure transitions. These edges deal with situations where, given an input symbol b and a state s, there is no forward transition from s using b. In such a case, the DFA should follow the failure transition to some state s' and take a forward transition from there. This process is repeated until a forward transition is found.


The following DFA was constructed for patterns set {E, BE, BD, BCD, CDBCAB, BCAA}. Solid black edges are forward transitions while red scattered edges are failure transitions.
L(s) - the label of a state s, is the concatenation of symbols along the path from the root to s.
d(s) - the depth of a state s, is the length of the label L(s).

The failure transition from s is always to a state s', whose label L(s') is the longest suffix of L(s) among all other DFA states.

The DFA is traversed starting from root. When the traversal goes through an accepting state, it indicates that some patterns are a suffix of the input; one of these patterns always corresponds to the label of the accepting state.

Finally, we denote by scan(s,b), the AC procedure when reading input symbol b while in state s; namely, transiting to a new state s' after traversing failure transitions and a forward transition as necessary, and reporting matched patterns in case s'.output != emptyset. scan(s,b) returns the new state s' as an output.

### Program Description
The behavior of the pattern matching machine is dictated by three functions:
1. a goto function g - maps a pair consisting of a state and an input symbol into a state or the message fail
2. a failure function f - maps a state into a state, and is consulted whenever the goto function reports fail
3. an output function output - associating a set of keyword (possibly empty) with every state

Constructing the FSM:
1. Determine the states and the goto function
2. Compute the failure function
3. Output function start at first step, complete at the second step

### included file
1) slist.c -  Implemention of generic link list
  the link list supports:  initialization, adding element ( to the head/tail) , removing firs element, and apendining two lists.
2) slist.h -  Declares the functions that we use in "slist.c" and nedeed structures.
3) pattern-matching.c- Implemention of the method to create one fsm.
		the fsm soupport : initialization, creating, search, destroy  fsm. 
4) Makefile – handle code compilation.
