# AVL Trees

AVL trees are one of the earliest forms of self-balancing binary search trees in theoretical computer science. They
ensure efficient search, insertion, and deletion operations, all of which are guaranteed to run in O(log(n)) time
complexity in the worst-case scenario. The balancing property of AVL trees is maintained through rotations, ensuring
that the height difference between the left and right subtrees (known as the balance factor) is kept within a certain
range.

## Key Properties

* Binary Search Tree: Like all binary search trees, an AVL tree maintains the property that for each node, all elements
  in its left subtree are less than the node's value, and all elements in its right subtree are greater.

* Balanced Height: An important characteristic of AVL trees is their balance factor, which is the absolute difference in
  height between the left and right subtrees of any node. This difference is always kept less than or equal to 1 for
  every node in the tree. In other words, the AVL always verifies `abs(h(LSB) - h(RSB)) < 2`; where h(LSB) is the height
  of the left subtree and h(RSB) is the height of the right subtree.

## Optimisation (Calculating Balance from previous Balances)

An optimization technique commonly used in AVL trees involves calculating the balance factor of a node based on the
previous balance (before the rotation). This optimization helps reduce the complexity of rebalancing the tree during
insertion and deletion operations, improving overall performance.
![Left Rotation](../img/left_rotation.png)
![Right Rotation](../img/right_rotation.png)