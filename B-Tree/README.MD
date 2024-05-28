# B-Tree
https://www.youtube.com/watch?v=K1a2Bk8NrYQ&t=13s

The insertion and deletion are optimised to only happen in one pass in order to reduce the accesses to the disk, which is the whole purpose of using a B-Tree.
This is achieved by making sure while going down towards the concerned node that:
- if inserting: it doesn't have more than 2m-1 keys
- if deleting: it doesn't have less than m keys