# Lottery-Scheduling-Algorithm

A lottery scheduling algorithm is a fascinating approach to CPU process scheduling and process management. It is known as a **proportional share** approach, also sometimes referred to as a **fair-share** scheduler. The general idea is that instead of optimizing for turnaround time
or reponse time, a schedular might instead try to gurantee that each job obtain a certain percentage of CPU time. Tshe baic idea of a lottery
scheduler is quite simple: every so often a lottery is held to determine which processs should run next. Processes that run more oftem should be
given more chances to win the lottery. 

Underlying lottery scheduling is one simple concept: **tickets**, which are used to represent the ahare of a resource that a process (or user or whatever) should receive. The percent of tickets that a process has represents its share of the resource in question. Let's look at an exammple. Imagine two processes, A and B, and imagine that A has 75 tickets while B has 25. Thus, what we would like is for A to receive 75% of the CPU and B the remaining 25%. Lottery scheduling acheives this probabilistically (non-deterministically) by holding a lottery every so oftem (pehaps every time slice). Holding a lottery is straightforward: the ccheduler must know how many total tickets there are (in our example, there are 100). The scheduler then picks a winning ticket, which is a number from 0 to 99. Assuming A holds tickets 0 through 74 and B 75 to 99, the winning ticket simply determines whether A or B runs. The schduler than loads the state of that winning process and runs it. 

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
   traditional fair-share algorithm. tracking how much CPU each process has received requires per-process accounting, which must be updated
   after running each process. Doing so randomly necessitates omly the most minimal of per-process state. Finally. random can be extremely fast.
   As long as generating a random number is quick, making the decision is also, and thus random can be used in a number of places where speed
   is requitred. Of course, the faster the need, the more random tends toward pseudo-random.

    Lottery scheduling also provides a number of mechanisms to manipulate tickets in different and sometimes useful ways. One way is with the 
    conecpt of **ticket currency**. Currency allows a user with a set of tickets to allocate tickets among their own jobs in whataver currency
    they would like; the system then automatically converts said currency into the correct global value. For example, assume users A and B each
    have been given 100 tickets. User A is running two jobs, A1 and A2, and gives them each 500 tickets (out of 1000 total) in user A's owm
    currency. User B us running onlt 1 job and gives it 10 tickets (out of 10 total). The system will convert A1's and A2's allocation from 500
    each in A's currency to 50 each in the global currency. Similarly, B1's 10 tickets will be converted to 100 tikctes. The lottery will then
    be held over the global ticket currency (200 total) to determine which job runs.

      User A -> 500 (A's currency
    
    
    
   
 
