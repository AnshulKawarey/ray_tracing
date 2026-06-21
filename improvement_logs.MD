### Original Approach (single threaded no optimisation):
- SD:
    ```
    init:   0ms
    Scanning remaining: 1 Render + write (same op):   11731ms
    Done.  
    ```
- HD:
    ```
    init:   0ms
    Scanning remaining: 1 Render + write (same op):   270956ms
    Done.  
    ```

### OpenMP multithreaded approach with same base rendering

- Parallelised the render loop across CPU cores with OpenMP.
- SD: 11731ms → 4292ms (~2.7x)
- HD: 270956ms → 98103ms (~2.8x)

- SD:
    ```
    init:   0ms
    render:   4292ms
    write:   27ms
    Done.   
    ```
- HD:
    ```
    init:   0ms
    render:   98103ms
    write:   657ms
    Done.   
    ```

### Double to Float

- Switched double to float. CPU can fit 2x more floats in the same register width,
  and SIMD units process twice as many per instruction.
  GPU shaders are also optimised for float, so this is necessary groundwork for Graphics APIs later.

- SD:
    ```
    init:   0ms
    render:   3929ms
    write:   27ms
    Done. 
    ```

- HD:
    ```
    init:   0ms
    render:   89446ms
    write:   662ms
    Done.  
    ```

### Float + Multi + Tile-Based

- Changed work unit from rows to 16x16 tiles.
  Two effects: finer parallel granularity (375 tiles vs 225 rows gives better load balancing),
  and spatial locality for BVH traversal, nearby pixels traverse the same nodes.
  Cache benefit is mostly theoretical until BVH is added; speedup here is from parallelism granularity.
- SD: 3929ms → 3383ms (~14%)
- HD: 89446ms → 76475ms (~14%)

- SD:
    ```
    init:   0ms
    375/375 tiles
    render: 3383ms
    write:  29ms
    ```
- HD:
    ```
    init:   0ms
    8160/8160 tiles
    render: 76475ms
    write:  727ms
    Done.  
    ```
