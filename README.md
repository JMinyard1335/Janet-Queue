# Janet-Queue

Implementation of the Queue Data Structure in the Janet Programming Language. 

## Installation
To Install using jpm run the following command in your terminal
#+begin_src bash
git clone <repo> <dir>
cd <dir>
jpm build
jpm test
sudo jpm install
#+end_src

## API
Below are the api calls currently implemented.
### (new ...args)
Creates a new queue from the given arguments. If no arguments are given then an empty queue is created
#+begin_src scheme
(queue/new 1 2 3 4) -> [1 2 3 4]
(queue/new @{"age" 15} 15 "Coffee")
(queue/new) -> []
#+end_src

### (empty? queue)
Checks to see if the given queue is empty
#+begin_src scheme
(queue/empty? (queue/new 1 2 3 4)) -> false
(queue/empty? (queue/new)) -> true
#+end_src

### (length queue)
Returns the length of the given queue
#+begin_src scheme
(queue/length (queue/new 1 2 3 4)) -> 4
(queue/length (queue/new)) -> 0
#+end_src

### (enqueue queue value)
Adds 'value' to the back of 'queue'
#+begin_src scheme
(queue/enqueue (queue/new) 1) -> [1]
(queue/enqueue (queue/new 1 2 3 4) 5) -> [1 2 3 4 5]
#+end_src

### (dequeue queue)
Removes the value from the front of 'queue' and returns it.
#+begin_src scheme
(queue/dequeue (queue/new)) -> nil
(queue/dequeue (queue/new 1 2 3 4)) -> [2 3 4]
#+end_src

### (peek-front queue)
Checks the Value at the front of the queue with out removing or modifying the queue.
#+begin_src scheme
(queue/peek-front (queue/new)) -> nil
(queue/peek-front (queue/new 1 2 3 4)) -> 1
#+end_src

### (peek-back queue)
Checks the Value at the front of the queue with out removing or modifying the queue.
#+begin_src scheme
(queue/peek-back (queue/new)) -> nil
(queue/peek-back (queue/new 1 2 3 4)) -> 4
#+end_src

### (to-array queue)
Converts a Queue into a JanetArray.
#+begin_src scheme
(queue/peek-back (queue/new)) -> @[]
(queue/peek-back (queue/new 1 2 3 4)) -> @[1 2 3 4]
#+end_src
