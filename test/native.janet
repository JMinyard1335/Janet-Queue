(import queue :as q)


(defn test-new []
  (print "Running New Tests...")
  (print "Running Test 1: Empty Queue")
  (def q (q/new))
  (assertf (= (q/length q) 0) "Queue Should be empty length should be 0. length = %v" (q/length q))
  (assert (= (q/peek-back q) nil) "There Should be no elements in the queue.")
  (print "Running Test 2: Construct Queue")
  (def q1 (q/new 0 1 2 3 4 5 6 7))
  (assert (= (q/length q1) 8) "Elements not added to the queue")
  (loop [i :range [0 (q/length q1)]]
    (assert (= (get q1 i) i) "Element Not added correctly."))
  (print "All 'New' Tests Passed!\n"))


(defn test-empty []
  (print "Running 'Empty' Tests...")
  (print "Running Test 1: Empty Queue")
  (def q1 (q/new))
  (assert (true? (q/empty? q1)) "Queue is empty and is not flagged as empty.")
  (print "Running Test 2: Non Empty Queue")
  (def q (q/new 1 2 3 4))
  (assert (false? (q/empty? q)) "Queue is not empty but has been flagged as empty.")
  (print "All 'Empty' Tests Passed!\n"))


(defn test-length []
  (print "Running 'Length' Tests...")
  (def q (q/new 1 2 3))
  (assert (= 3 (q/length q)) "Length is not stored properly")
  (print "All 'Length' Tests Passed!\n"))


(defn test-peeks []
  (print "Running 'Peek' Test...")
  (def q (q/new 1 2 3 4))
  (print "Running Test 1: Peek-Front")
  (assert (= (q/peek-front q) 1) "Not returning the correct front")
  (print "Running Test 2: Peek-Back")
  (assert (= (q/peek-back q) 4) "Not returning the correct back")
  (print "All 'Peak' Tests Passed!\n"))


(defn test-enqueue []
  (def queue (q/new))
  (print "Running 'Enqueue' Tests...")
  (print "Running Test 1: Single Enqueue...")
  (q/enqueue queue 1)
  (assert (= (get queue (- (q/length queue) 1)) 1) "Enqueue Failed")
  # (print "Running Test 2: Multi Enqueue...")
  # (q/enqueue queue 2 3 4)
  # (assert (= (get queue (- (q/length queue) 1)) 4) "Enqueue Failed")
  (print "All 'Enqueue' Tests Passed!\n"))


(defn test-dequeue []
  (def q (q/new))
  (def q1 (q/new 1 2))

  (print "Running 'Dequeue' Tests...")
  (print "Running Test 1: dequeue from empty")
  (var rv (q/dequeue q))
  (assert (nil? rv) "Dequeueing from an empty queue should return nil.")
  (assert (= 0 (q/length q)) "Dequeueing from empty size should still be 0")
  
  (print "Running Test 2: dequeue single element")
  (set rv (q/dequeue q1))
  (assert (= rv 1) "Wrong Value was dequeued")
  (assert (= (get q1 0) 2) "Correct Value was not found left after dequeue")
  (print "All 'Dequeue' Tests Passed!\n"))


(defn test-to-array []
  (print "Running 'To-Array' Tests...")
  (def q (q/new 1 2 3))
  (def a (q/to-array q))
  (assert (deep= a @[1 2 3]) "Failed to convert queue to array")
  (print "All 'To-Array' Tests Passed!"))

(test-new)
(test-empty)
(test-length)
(test-peeks)
(test-enqueue)
(test-dequeue)
(test-to-array)
