(declare-project
  :name "queue"
  :description ```An implementation of a queue using the <sys/queue.h> header in c. ```
  :version "0.0.0")

(declare-native
  :name "queue"
  :source ["c/module.c"])
