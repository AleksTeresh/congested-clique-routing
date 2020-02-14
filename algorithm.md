
# Overview

This report describes the congested clique routing algorithm. While [the original paper](https://people.mpi-inf.mpg.de/~clenzen/pubs/L13optimal.pdf) provides a detailed description of the algorithm, it is perhaps targeted more towards understanding the ideas on the conceptual level (i.e. what is happening) as opposed to implementation detail (how to make it happen). In particular, it seems that Step 2 of Algorithm 1 could be described more explicitly if the goal of the description would be the implementation of the algorithm. What follows is a humble attempt at describing the routing algorithm in plain English s.t. it is easy to translate it to code.

The description follows notation similar to that of the [the original paper](https://people.mpi-inf.mpg.de/~clenzen/pubs/L13optimal.pdf). A subset of nods in its sending role is denoted as $W$ and a subset in its receiving role by $W'$.

Finally, the algorithm description below is, obviously, heavily based on [the paper](https://people.mpi-inf.mpg.de/~clenzen/pubs/L13optimal.pdf). In particular, certain sentences or paragraphs from the paper were directly used in the description.

# Assumptions

Note that in the description below, we assume that the number of nodes in a single subset is exactly $\sqrt n$ (which is an integer) for each subset, and there are $\sqrt n$ subsets in total. Besides, each of the nodes sends and receives exactly $n$ messages. These constraints are introduced for clarity and can be relaxed with trivial modifications to the algorithm.

We also assume that each node has a unique ID.

Each of the following steps are described from a perspective of a single node. That is each of the rounds described below is supposed to be executed
independently by each of the $n$ nodes concurrently. Once all the nodes are done with a round, the next round is started. Finally, each node can send exactly one message to each other node during a single round.

Besides, we assume that Corollary 3.3 and Corollary 3.4 are available as subroutines, hence the details of their implementation are not going to be described.

# Algorithm

## Step 1

Partition the nodes into the disjoint subsets $\{(i-1)\sqrt n + 1, ..., i \sqrt n\}$ for $i \in \{1, ..., \sqrt n\}$.

Throughout the rest of the section, we assume that subgroups and nodes within each subgroup have a unique ordering s.t. they can be referred to by their index (e.g. index of a subgroup or index of a node within some subgroup).

## Step 2

The step moves all the messages s.t. each subset $W$ holds exactly $|W||W'| = n$ messages for each subset $W'$

### Round 1

1. Calculate how many messages the node holds are destined for each subset.

2. Send the number of messages destined to $i'th$ subset to $i'th$ node in the current subset $W$

### Round 2

1. Sum up figures sent by other nodes to the current node in the previous step.

2. The result (a single summed up value) is announced to all the $n$ nodes in the clique.

### Round 3
Now each node has information about how many messages each subset $W$ needs to send to each subset $W'$

1. Construct a bipartite graph $G = (S \  \mathbin{\dot{\cup}}  R, \  E)$ s.t. S and R represent subsets $W$ and $W'$ respectively and an edge is added from $s \in S$ to $r \in R$ if there is a message to be sent from corresponding $W$ to $W'$.

Every vertex in the graph happens to have a degree of $n^{3/2}$ since there are $\sqrt n$ nodes in each subset each with $n$ messages to be sent.

2. Color the graph with $n^{3/2}$ colors.

3. Save the graph for future use.

4. Calculate how many messages the node holds are destined for each subset. (the same that was calculated in Round 1 of the current step)

5. Send $\sqrt n$ computed numbers to each of the nodes in the current subset. (Can be done in 2 rounds using Corollary 3.3)

### Round 4

The second round of Corollary 3.3. This just finalizes the previous round.

Now, each node in $W$ knows how many messages all other nodes in $W$ have destined to each of the subsets $W'$

### Round 5

1. Count how many messages each of the nodes in $W$ has for each of the subsets $W'$.

2. Construct a bipartite graph $G' = (S \  \mathbin{\dot{\cup}} \  R, \  E)$ s.t. S and R represent nodes in the current subset and subsets $W'$ respectively. We then use the coloring of the graph constructed in Round 3 as follows. 
   1. For each destination subset $W'$, for each edge $e$ in $G$ from current subset to $W'$, get color $c$ of the edge
   2. Get subset index as $i = c \mod \sqrt n$. This subset $i$ will be an intermediate destination for a message represented by the edge $e$.
   3. Find a message (among nodes of the current subset) that has $i'th$ subset as its destination. Since there is an edge in $G$ for it, some node $v$ in the current subset is going to have such a message.
   4. Add an edge to $G'$ from $s \in S$ corresponding to $v$ to $r \in R$ corresponding to $i'th$ subset.
   5. The resulting graph $G'$ is $n$-regular bipartite.

3. Color the graph $G'$ with $n$ colors.

4. Now construct one more graph $G'' = (S \  \mathbin{\dot{\cup}} \  R, \  E)$ where $S$ and $R$ represent nodes of the current subset in their sending and receiving role.
   1. For each node $v$ in the current subset (as sender), for each subset $q$, for each edge $e$ in $G'$ s.t. $e$ goes from $v$ to $r$, get the color $c$ of $e$
   2. Get local index of a node in the current subset as $i = c \mod \sqrt n$
   3. Add an edge to $G''$ from $s \in S$ corresponding to $v$ to $r \in R$ corresponding to $i'th$ node in the current subset.
   4. According to $G''$, send messages within current subset using Corollary 3.3 (in 2 rounds)

### Round 6

The second round of Corollary 3.3. This just finalizes the previous round.

### Round 7

By now we've achieved that each node holds exactly $\sqrt n$ messages with the destination of $W'$ (for each $W'$). Thus, the current node can send one message to each of the $\sqrt n$ nodes of W' (for each W'). => $\sqrt n * \sqrt n = n$ messages are sent in total by each node.

## Step 3

In this step, for each pair of subsets $W$ and $W'$, we move messages within $W$ s.t. each node in $W$ holds exactly $|W'| = \sqrt n$ messages with destination in $W'$.

### Round 1

1. Count how many messages this node has for each of the subsets

2. Announce the figures to all nodes in the current subset. Using Corollary 3.3, announce $\sqrt n$ numbers to $\sqrt n$ nodes within the current subset.

### Round 2

2nd round of Corollary 3.3. This just finalizes the previous round.

### Round 3

By now each node in each subset $W$ knows for each other node in the subset, how many messages they have for each of the subsets $W'$

1. For each node $v$ of the current subset, for each subset $W'$, make sure there are $\sqrt n$ messages to be sent from $v$ to $W'$. We do so by constructing a graph $G$ and then sending messages according to it
   1. While $v$ has fewer than $\sqrt n$ messages with the destination in $W'$, find any node $u$ in the current subset that has more than $\sqrt n$ messages with destination to $W'$ (such node is guaranteed to exist by Step2).
   2. Add an edge to $G$ from $s \in S$ representing $u$ to $r \in R$ representing $v$.
   3. We also need to update our bookkeeping s.t. if we had that $u$ had $\sqrt n + 1$ messages destined to $W'$, next round of "search" for extra messages would show that $u$ only has $\sqrt n$ messages left.

2. According to the directed graph $G$, use Corollary 3.3 to send messages between nodes of the current subsets according to the edges in $G$.

### Round 4

2nd round of Corollary 3.3. This just completed the previous round.

## Step 4

This step does the following. If there are messages in some subset $W$ with the destination for some subset $W'$ s.t. $W  \neq W'$, send those messages from $W$ to $W'$. Because of the previous step, this can be done in a single round.

1. Initialize an array of $\sqrt n$ indices (one per subset) that would indicate the last node in each subset that the current node has already sent a message too. The idea here is that the given node has at most $\sqrt n$ messages to send to each subset, so sending one to each node will be sufficient.

2. For each message held by the node, get an index $i$ of a subset $|W'|$ to which this message is destined. Then get an index $j$ of the node in that subset $|W'|$ to which some previous message was sent during the round (if any). Finally, send the message to $(j+1)'th$ node of $i'th$ subset and update our bookkeeping array.

## Step 5

At this point, all the messages in the clique are located in the subsets which they are destined for.

In this step, we just use Corollary 3.4 to send messages within each subset to their final destination nodes in 4 rounds.
