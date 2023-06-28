# Lottery-Scheduling-Algorithm

A lottery scheduling algorithm is a fascinating approach to CPU process scheduling and process management. It is known as a **proportional share** approach, also sometimes referred to as a **fair-share** scheduler. The general idea is that instead of optimizing for turnaround time
or response time, a schedular might instead try to guarantee that each job obtain a certain percentage of CPU time. The baic idea of a lottery
scheduler is quite simple: every so often a lottery is held to determine which processs should run next. Processes that run more often should be
given more chances to win the lottery. 

Underlying lottery scheduling is one simple concept: **tickets**, which are used to represent the share of a resource that a process (or user or whatever) should receive. The percent of tickets that a process has represents its share of the resource in question. Let's look at an example. Imagine two processes, A and B, and imagine that A has 75 tickets while B has 25. Thus, what we would like is for A to receive 75% of the CPU and B the remaining 25%. Lottery scheduling acheives this probabilistically (non-deterministically) by holding a lottery every so oftem (pehaps every time slice). Holding a lottery is straightforward: the scheduler must know how many total tickets there are (in our example, there are 100). The scheduler then picks a winning ticket, which is a number from 0 to 99. Assuming A holds tickets 0 through 74 and B 75 to 99, the winning ticket simply determines whether A or B runs. The schduler than loads the state of that winning process and runs it. 

Here is an example output of a lottery scheduler's winning tickets:

 63 85 70 39 76 17 29 41 36 39 10 99 68 83 63 62 43 0 49 49

 Here is the resulting schedule:
 
   A B A A B A A A A A B A B A A A A A A A

   As can be seen from the example, the use of randomness in lottery scheduling leads to a probabilistic correctness in meeting the desired
   proportion, but no guarantee. In our example above, B only gets to run 4 out of 20 times (20%), intead of the desired 25% allocation.
   However, the longer these two jobs compete, the more likely they are to acheive the desired percentages over time.

   One of the most beautiful aspects of lottery scheduling is its use of **randomness**. When you have to make a decision, using such a 
   randomized approach is often a robust and simple way of doing things. Random approaches have at least three advantages over more traditional
   approaches. First, random often avoids strange corner-case behaviors that a more traditonal algorithm may have trouble handling.For example,
   consider LRU page replacement; while oftem a good replacement algorithm, LRU perfoms pessimally for some cyclic-sequential workloads. Random.
   on the other hand, has no such worst case. Second, random is also lightweight, requirirng little state to track alternatives. In a 
   traditional fair-share algorithm, tracking how much CPU each process has received requires per-process accounting, which must be updated
   after running each process. Doing so randomly necessitates only the most minimal of per-process state. Finally. random can be extremely fast.
   As long as generating a random number is quick, making the decision is also, and thus random can be used in a number of places where speed
   is requitred. Of course, the faster the need, the more random tends toward pseudo-random.

   Lottery scheduling also provides a number of mechanisms to manipulate tickets in different and sometimes useful ways. One way is with the 
   conecpt of **ticket currency**. Currency allows a user with a set of tickets to allocate tickets among their own jobs in whataver currency
   they would like; the system then automatically converts said currency into the correct global value. For example, assume users A and B each
   have been given 100 tickets. User A is running two jobs, A1 and A2, and gives them each 500 tickets (out of 1000 total) in user A's owm
   currency. User B us running onlt 1 job and gives it 10 tickets (out of 10 total). The system will convert A1's and A2's allocation from 500
   each in A's currency to 50 each in the global currency. Similarly, B1's 10 tickets will be converted to 100 tickets. The lottery will then
   be held over the global ticket currency (200 total) to determine which job runs.

   Another useful mechanism is **ticket transfer**. With transfers, a process can temporarily hand off its tickets to another process. This 
   ability is especially useful in a client/server setting, where a client process sends a message to a server asking it to do some work on 
   the client's behalf. To speed up the work, the client can pass the tickets to the server and thus try to maximize the performance of the
   server while the server is handling the client's request. When finished, the server then transfers the tickets back to the client and all
   is as before.

   ## Implementation

   Probably the most amazing thing about lottery scheduling is the simplicity of its implementation. All you need is a good random number 
   generator to pick the winning ticket, a data structure to track the processes of the system (e.g. a list), and the total number of
   tickets. Let's assume we keep the processes in a list. Here is an example composed of three processes, A,B, and C, each with some number
   of tickets.

     head -> Job:A (Tix:100)  -> Job:B (Tix:50) -> Job C (Tix: 250) -> NULL

   To make a scheduling decision,we first have to pick a random number (the winner) from the total number of tickets (400). Let's say we
   pick the number 300. Then, we simply traverse the list, with a simple counter used to help us find the winner.

   ```C
    // counter used to track if we've found the winner yet
    int counter =0:
    
    //winner: use some call to a random number generator to get a value between 0 and 
    // the total number of tickets
     int winner = getrandom(0,totaltickets);

     // current : use this to walk through the list of jobs
     node_t  *current = head;

      //loop until the sum of the ticket values is > the winner
      while (current) {
          counter=counter + current->tickets;
          if (counter > winner)
              break; //founf the winner
           current= current-> next: 
            }  
          
   ```
          
   The code walks through the list of processes, adding each ticket value to *counter* until the value exceeds *winner*. Once that is the 
   case, the current list element is the winner. With our example of the winning ticker being 300, the following takes place. First.
   *counter* is incremented to 100 to account for A's tickets. Since 100 us less than 300, the loop continues. Then *counter* would be 
   updated to 150 (B's tickets), still less than 300 and thus again we continue. Finally, *counter* is updated to 400 (greater than 300)
   and thus we break out of the loop with *current* pointing at C (the winner).
   
   To make this process most efficient, it might be generally best to organize the list in sorted order, from the highest number of tickets
   to the lowest. The ordering does not affect the correctness of the algorithm. but it does ensure in general that the fewest number of
   list iterations are taken, especially if there are a few processes that possess most of the tickets.
     
   To make the dynamics of lottery scheduling more understandable, we now perform a breif study of the completion time of two jobs 
   competing with one another, each with the same number of tickets (100) and same run time (R). In this scenario, we would like for
   each job to finish at roughly the same time, but due to the randomness of lottery scheduling, sometimes one job finishes before the
   other. To quantify the difference, we define a simple **unfairness metric**, U, which is simply the time the first job completes 
   divided by the time that the second job completes. For example, if R=10, and the first job finishes at time 10 (and the second at time
   20) U= 0.5. When both jobs finish at nearly the same time, U will be quite close to 1. In this scenario, that is our goal: a perfectly
   fair scheduler would achieve U =1. When the job length is not very long, average unfairness can be quite severe. Only as the jobs run
   for a significant number of time slices does the lottery scheduler approach the desired outcome. 


   
   
    
    
    
   
 
