# Entity Grouping Algorithm

## Overview
This project implements an algorithm to find two sublists from a given list of `Entity` objects that satisfy the following constraints:

1. Each sublist has a total **count** sum of exactly **5**.
2. The **weighted average strength** of each sublist, computed as:

   $$
   wavg = \frac{\sum (strength_i \times count_i)}{5} 
   $$
   
   should be as close as possible between the two selected sublists.

## Approach
The algorithm follows these steps:

### 1. Generate Candidate Sublists
- Iterate through the list of `Entity` objects.
- Construct sublists by adding elements until the total **count** reaches 5.
- If **count exceeds 5**, backtrack and try another combination.
- If **count equals 5**, store the sublist as a valid candidate.
- Continue until all possible valid sublists are found.

### 2. Compute Weighted Averages
 For each valid sublist, compute its weighted average strength:
   
   $$
   wavg = \frac{\sum (strength_i \times count_i)}{5} 
   $$

 Store all `wavg` values.

### 3. Calculate Mean and Standard Deviation
  Compute the **mean** of all weighted averages:
  
   $$
   mean_w = \frac{1}{n} \sum wavg_i
   $$

  Compute the **standard deviation**:
  
  $$
  \sigma = \sqrt{\frac{1}{n} \sum (wavg_i - \text{mean_w})^2}
  $$

### 4. Select Two Groups
  Identify two sublists `(i, j)` where:
  
  $$
  |wavg_i - wavg_j| < \sigma
  $$
  
 This ensures that their weighted averages are statistically similar and minimizes imbalance.

### 5. Fallback Strategy: Shuffling & Retrying
- If no valid sublist pairs are found, the algorithm **shuffles** the initial list and retries using the same approach.
- This increases the chances of finding valid groups by exploring different orderings.
- The process repeats a limited number of times before concluding that no solution exists.

## Usage
1. Clone the repository:
   ```sh
   git clone git@github.com:cmoustakas/ListTask.git
   cd ListTask
   ./build_n_run_tests.sh
   ```
## License
This project is open-source and available under the [MIT License](LICENSE).

