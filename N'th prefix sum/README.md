
```
EXAMPLE:
       data     |  a  |       b  |             c  |                   d
----------------|-----|----------|----------------|----------------------
  I prefix sum  |  a  |   a + b  |    a +  b + c  |    a +   b +  c + d
 II prefix sum  |  a  |  2a + b  |   3a + 2b + c  |   4a +  3b + 2c + d
III prefix sum  |  a  |  3a + b  |   6a + 3b + c  |  10a +  6b + 3c + d
 IV prefix sum  |  a  |  4a + b  |  10a + 4b + c  |  20a + 10b + 4c + d
```
