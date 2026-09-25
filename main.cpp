#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// ============================================================
// Problem 1: Merge Overlapping Intervals
// ============================================================

// Compare intervals by their starting value only
bool compareFirstElement(const vector<int>& a,
                         const vector<int>& b) {
    return a[0] < b[0];
}

// Merges all overlapping intervals.
vector<vector<int>> mergeIntervals(vector<vector<int>> intervals) {
    if (intervals.empty()) {
        return {};
    }

    // Sort intervals by their starting value.
    sort(intervals.begin(), intervals.end(), compareFirstElement);

    vector<vector<int>> merged;
    vector<int> current = intervals[0];

    for (int i = 1; i < intervals.size(); ++i) {
        vector<int> next = intervals[i];

        // The intervals overlap.
        if (next[0] <= current[1]) {
            current[1] = max(current[1], next[1]);
        } else {
            merged.push_back(current);
            current = next;
        }
    }

    merged.push_back(current);
    return merged;
}

// ============================================================
// Problem 2: Sort an Array Containing 0, 1, and 2
// Counting solution
// ============================================================

vector<int> sort012_counting(vector<int>& items) {
    int counter[3] = {0, 0, 0};

    for (int i = 0; i < items.size(); i++) {
        counter[items[i]]++;
    }

    int index = 0;

    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j < counter[i]; j++) {
            items[index] = i;
            index++;
        }
    }

    return items;
}

// Problem 2: Dutch National Flag solution
vector<int> sort012_dutch(vector<int>& items) {
    int low = 0;
    int mid = 0;
    int high = items.size() - 1;

    while (mid <= high) {
        if (items[mid] == 0) {
            swap(items[low], items[mid]);
            low++;
            mid++;
        } else if (items[mid] == 1) {
            mid++;
        } else {
            swap(items[mid], items[high]);
            high--;
        }
    }

    return items;
}

// ============================================================
// Problem 3: Largest Number
// ============================================================

string largestNumber(const vector<int>& numbers) {
    vector<string> numberStrings;

    // Convert every number to a string.
    for (int i = 0; i < numbers.size(); i++) {
        numberStrings.push_back(to_string(numbers[i]));
    }

    if (numberStrings.empty()) {
        return "";
    }

    string result;

    // Continue until all numbers have been selected.
    while (!numberStrings.empty()) {
        // Assume the first remaining number is the best choice.
        int bestIndex = 0;

        // Find the number that should come next.
        for (int i = 1; i < numberStrings.size(); i++) {
            string firstOrder =
                numberStrings[i] + numberStrings[bestIndex];

            string secondOrder =
                numberStrings[bestIndex] + numberStrings[i];

            if (firstOrder > secondOrder) {
                bestIndex = i;
            }
        }

        // Add the selected number to the result.
        result += numberStrings[bestIndex];

        // Remove the selected number.
        numberStrings.erase(numberStrings.begin() + bestIndex);
    }

    // If all numbers were zero, return a single zero.
    if (result[0] == '0') {
        return "0";
    }

    return result;
}

// ============================================================
// Problem 4: Group Anagrams
// ============================================================

vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> anagramGroups;

    for (int i = 0; i < strs.size(); i++) {
        string sortedString = strs[i];

        sort(sortedString.begin(), sortedString.end());

        anagramGroups[sortedString].push_back(strs[i]);
    }

    vector<vector<string>> result;

    for (const auto& [key, anagrams] : anagramGroups) {
        result.push_back(anagrams);
    }

    return result;
}

// ============================================================
// Problem 5: Meeting Conflicts
// ============================================================

bool canAttendMeetings(vector<vector<int>>& meetings) {
    if (meetings.empty()) {
        return true;
    }

    sort(meetings.begin(), meetings.end());

    for (int i = 1; i < meetings.size(); i++) {
        if (meetings[i][0] < meetings[i - 1][1]) {
            return false;
        }
    }

    return true;
}

// ============================================================
// Problem 6: K Closest Points
// Sorting-based solution
// ============================================================

vector<vector<int>> kClosestUsingSorting(
    vector<vector<int>>& points, int k) {

    vector<pair<long long, vector<int>>> pointsWithDistances;

    for (int i = 0; i < points.size(); i++) {
        long long distance =
            1LL * points[i][0] * points[i][0] +
            1LL * points[i][1] * points[i][1];

        pointsWithDistances.push_back(
            {distance, points[i]}
        );
    }

    sort(pointsWithDistances.begin(), pointsWithDistances.end());

    vector<vector<int>> result;

    for (int i = 0; i < k; i++) {
        result.push_back(pointsWithDistances[i].second);
    }

    return result;
}

// Problem 6: Heap-based solution
vector<vector<int>> kClosestUsingHeap(
    vector<vector<int>>& points, int k) {

    priority_queue<
        pair<long long, vector<int>>,
        vector<pair<long long, vector<int>>>,
        greater<pair<long long, vector<int>>>
    > minHeap;

    for (int i = 0; i < points.size(); i++) {
        long long distance =
            1LL * points[i][0] * points[i][0] +
            1LL * points[i][1] * points[i][1];

        minHeap.push({distance, points[i]});
    }

    vector<vector<int>> result;

    for (int i = 0; i < k; i++) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }

    return result;
}

// ============================================================
// Problem 7: K Most Frequent Values
// ============================================================

// Sort by higher frequency first.
// If frequencies tie, sort by smaller number first.
bool compareFrequency(const pair<int, int>& a,
                      const pair<int, int>& b) {
    if (a.first != b.first) {
        return a.first > b.first;
    }

    return a.second < b.second;
}

vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> count;

    for (int i = 0; i < nums.size(); i++) {
        count[nums[i]]++;
    }

    vector<pair<int, int>> arr;

    // Each pair is stored as {frequency, number}.
    for (const auto& entry : count) {
        arr.push_back({entry.second, entry.first});
    }

    // Use the custom comparator to define the required order.
    sort(arr.begin(), arr.end(), compareFrequency);

    vector<int> result;

    for (int i = 0; i < k; i++) {
        result.push_back(arr[i].second);
    }

    return result;
}

// ============================================================
// Problem 8: Merge K Sorted Arrays
// Pointer-scanning solution
// ============================================================

vector<int> mergeKArrays(vector<vector<int>>& arrays) {
    int k = arrays.size();

    // One pointer for each array
    vector<int> pointers(k, 0);

    vector<int> result;

    // Continue until all arrays have been completely traversed
    while (true) {
        int minValue = INT_MAX;
        int minArray = -1;

        // Find the smallest current element among the k arrays
        for (int i = 0; i < k; i++) {
            // Make sure this array still has elements
            if (pointers[i] < arrays[i].size()) {
                if (arrays[i][pointers[i]] < minValue) {
                    minValue = arrays[i][pointers[i]];
                    minArray = i;
                }
            }
        }

        // If no array has any elements left, we are done
        if (minArray == -1) {
            break;
        }

        // Add the minimum element to the result
        result.push_back(minValue);

        // Move the pointer of the array that provided the minimum
        pointers[minArray]++;
    }

    return result;
}

// ============================================================
// Problem 9: Kth Largest Element
// ============================================================

int findKthLargest(const vector<int>& nums, int k) {
    // Keep the k largest values in a min heap.
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int i = 0; i < nums.size(); i++) {
        minHeap.push(nums[i]);

        // Remove the smallest value if the heap has more than k values.
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    // The smallest value among the k largest values
    // is the kth largest value overall.
    return minHeap.top();
}

// ============================================================
// Problem 10: Count Inversions
// ============================================================

long long mergeAndCount(
    vector<int>& values,
    vector<int>& temporary,
    int left,
    int middle,
    int right) {

    int i = left;
    int j = middle + 1;
    int position = left;

    long long inversions = 0;

    while (i <= middle && j <= right) {
        // Equal values do not form an inversion.
        if (values[i] <= values[j]) {
            temporary[position] = values[i];
            i++;
        } else {
            temporary[position] = values[j];
            j++;

            // Every remaining value in the left half
            // is greater than values[j - 1].
            inversions += middle - i + 1;
        }

        position++;
    }

    // Copy remaining values from the left half.
    while (i <= middle) {
        temporary[position] = values[i];
        i++;
        position++;
    }

    // Copy remaining values from the right half.
    while (j <= right) {
        temporary[position] = values[j];
        j++;
        position++;
    }

    // Copy the merged section back into the original array.
    for (int index = left; index <= right; index++) {
        values[index] = temporary[index];
    }

    return inversions;
}

long long mergeSortAndCount(
    vector<int>& values,
    vector<int>& temporary,
    int left,
    int right) {

    if (left >= right) {
        return 0;
    }

    int middle = left + (right - left) / 2;

    long long inversions = 0;

    // Count inversions inside the left half.
    inversions += mergeSortAndCount(
        values, temporary, left, middle
    );

    // Count inversions inside the right half.
    inversions += mergeSortAndCount(
        values, temporary, middle + 1, right
    );

    // Count inversions crossing from the left half
    // into the right half.
    inversions += mergeAndCount(
        values, temporary, left, middle, right
    );

    return inversions;
}

long long countInversions(vector<int>& values) {
    if (values.empty()) {
        return 0;
    }

    vector<int> temporary(values.size());

    return mergeSortAndCount(
        values,
        temporary,
        0,
        values.size() - 1
    );
}

// ============================================================
// Test Output Helpers
// ============================================================

void printIntVector(const vector<int>& values) {
    cout << "[";

    for (int i = 0; i < values.size(); i++) {
        cout << values[i];

        if (i < values.size() - 1) {
            cout << ", ";
        }
    }

    cout << "]\n";
}

void printStringVector(const vector<string>& values) {
    cout << "[";

    for (int i = 0; i < values.size(); i++) {
        cout << "\"" << values[i] << "\"";

        if (i < values.size() - 1) {
            cout << ", ";
        }
    }

    cout << "]\n";
}

void printIntMatrix(const vector<vector<int>>& matrix) {
    cout << "[";

    for (int i = 0; i < matrix.size(); i++) {
        cout << "[";

        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j];

            if (j < matrix[i].size() - 1) {
                cout << ", ";
            }
        }

        cout << "]";

        if (i < matrix.size() - 1) {
            cout << ", ";
        }
    }

    cout << "]\n";
}

void printStringMatrix(const vector<vector<string>>& matrix) {
    cout << "[\n";

    for (int i = 0; i < matrix.size(); i++) {
        cout << "  [";

        for (int j = 0; j < matrix[i].size(); j++) {
            cout << "\"" << matrix[i][j] << "\"";

            if (j < matrix[i].size() - 1) {
                cout << ", ";
            }
        }

        cout << "]";

        if (i < matrix.size() - 1) {
            cout << ",";
        }

        cout << "\n";
    }

    cout << "]\n";
}

// ============================================================
// Main function
// Test cases for Problems 1-10
// ============================================================

int main() {
    // ========================================================
    // Problem 1: Merge Overlapping Intervals
    // ========================================================

    cout << "\n===== Problem 1: Merge Overlapping Intervals =====\n";

    // Test Case 1: Overlapping Intervals
    vector<vector<int>> problem1Case1 = {
        {1, 3}, {2, 6}, {8, 10}, {15, 18}
    };
    cout << "Test Case 1: ";
    printIntMatrix(mergeIntervals(problem1Case1));
    // Expected: [[1, 6], [8, 10], [15, 18]]

    // Test Case 2: Touching Intervals
    vector<vector<int>> problem1Case2 = {
        {1, 4}, {4, 5}
    };
    cout << "Test Case 2: ";
    printIntMatrix(mergeIntervals(problem1Case2));
    // Expected: [[1, 5]]

    // Test Case 3: Non-Overlapping Intervals
    vector<vector<int>> problem1Case3 = {
        {1, 2}, {4, 5}, {7, 9}
    };
    cout << "Test Case 3: ";
    printIntMatrix(mergeIntervals(problem1Case3));
    // Expected: [[1, 2], [4, 5], [7, 9]]

    // Edge Case: Empty Input
    vector<vector<int>> problem1Case4;
    cout << "Edge Case: ";
    printIntMatrix(mergeIntervals(problem1Case4));
    // Expected: []

    // ========================================================
    // Problem 2: Sort 0, 1, and 2
    // ========================================================

    cout << "\n===== Problem 2: Sort 0, 1, and 2 =====\n";

    // Test Case 1: Mixed Values
    vector<int> problem2CountingCase1 = {2, 0, 2, 1, 1, 0};
    vector<int> problem2DutchCase1 = problem2CountingCase1;

    sort012_counting(problem2CountingCase1);
    sort012_dutch(problem2DutchCase1);

    cout << "Test Case 1 - Counting: ";
    printIntVector(problem2CountingCase1);
    cout << "Test Case 1 - Dutch National Flag: ";
    printIntVector(problem2DutchCase1);
    // Expected: [0, 0, 1, 1, 2, 2]

    // Test Case 2: No 2s
    vector<int> problem2CountingCase2 = {0, 1, 1, 0, 1, 0};
    vector<int> problem2DutchCase2 = problem2CountingCase2;

    sort012_counting(problem2CountingCase2);
    sort012_dutch(problem2DutchCase2);

    cout << "Test Case 2 - Counting: ";
    printIntVector(problem2CountingCase2);
    cout << "Test Case 2 - Dutch National Flag: ";
    printIntVector(problem2DutchCase2);
    // Expected: [0, 0, 0, 1, 1, 1]

    // Test Case 3: Only One Category
    vector<int> problem2CountingCase3 = {2, 2, 2, 2};
    vector<int> problem2DutchCase3 = problem2CountingCase3;

    sort012_counting(problem2CountingCase3);
    sort012_dutch(problem2DutchCase3);

    cout << "Test Case 3 - Counting: ";
    printIntVector(problem2CountingCase3);
    cout << "Test Case 3 - Dutch National Flag: ";
    printIntVector(problem2DutchCase3);
    // Expected: [2, 2, 2, 2]

    // Edge Case: Empty Input
    vector<int> problem2CountingCase4;
    vector<int> problem2DutchCase4;

    sort012_counting(problem2CountingCase4);
    sort012_dutch(problem2DutchCase4);

    cout << "Edge Case - Counting: ";
    printIntVector(problem2CountingCase4);
    cout << "Edge Case - Dutch National Flag: ";
    printIntVector(problem2DutchCase4);
    // Expected: []

    // ========================================================
    // Problem 3: Largest Number
    // ========================================================

    cout << "\n===== Problem 3: Largest Number =====\n";

    // Test Case 1: Different Digit Lengths
    vector<int> problem3Case1 = {10, 2};
    cout << "Test Case 1: "
         << largestNumber(problem3Case1) << "\n";
    // Expected: "210"

    // Test Case 2: Multiple Numbers
    vector<int> problem3Case2 = {3, 30, 34, 5, 9};
    cout << "Test Case 2: "
         << largestNumber(problem3Case2) << "\n";
    // Expected: "9534330"

    // Test Case 3: Repeated Values
    vector<int> problem3Case3 = {12, 121};
    cout << "Test Case 3: "
         << largestNumber(problem3Case3) << "\n";
    // Expected: "12121"

    // Edge Case: All Zeros
    vector<int> problem3Case4 = {0, 0, 0};
    cout << "Edge Case - All Zeros: "
         << largestNumber(problem3Case4) << "\n";
    // Expected: "0"

    // Edge Case: Empty Input
    vector<int> problem3Case5;
    cout << "Edge Case - Empty Input: \""
         << largestNumber(problem3Case5) << "\"\n";
    // Expected: ""

    // ========================================================
    // Problem 4: Group Anagrams
    // ========================================================

    cout << "\n===== Problem 4: Group Anagrams =====\n";

    // Test Case 1: Multiple Anagram Groups
    vector<string> problem4Case1 = {
        "eat", "tea", "tan", "ate", "nat", "bat"
    };
    cout << "Test Case 1:\n";
    printStringMatrix(groupAnagrams(problem4Case1));
    // Expected groups:
    // ["eat", "tea", "ate"], ["tan", "nat"], ["bat"]

    // Test Case 2: No Anagrams
    vector<string> problem4Case2 = {
        "cat", "dog", "bird"
    };
    cout << "Test Case 2:\n";
    printStringMatrix(groupAnagrams(problem4Case2));
    // Expected groups: ["cat"], ["dog"], ["bird"]

    // Test Case 3: Repeated Anagrams
    vector<string> problem4Case3 = {
        "listen", "silent", "enlist", "listen"
    };
    cout << "Test Case 3:\n";
    printStringMatrix(groupAnagrams(problem4Case3));
    // Expected group:
    // ["listen", "silent", "enlist", "listen"]

    // Edge Case: Empty Input
    vector<string> problem4Case4;
    cout << "Edge Case:\n";
    printStringMatrix(groupAnagrams(problem4Case4));
    // Expected: []

    // ========================================================
    // Problem 5: Meeting Conflicts
    // ========================================================

    cout << "\n===== Problem 5: Meeting Conflicts =====\n";

    // Test Case 1: Overlapping Meetings
    vector<vector<int>> problem5Case1 = {
        {0, 30}, {5, 10}, {15, 20}
    };
    cout << boolalpha;
    cout << "Test Case 1: "
         << canAttendMeetings(problem5Case1) << "\n";
    // Expected: false

    // Test Case 2: Non-Overlapping Meetings
    vector<vector<int>> problem5Case2 = {
        {5, 10}, {15, 20}, {25, 30}
    };
    cout << "Test Case 2: "
         << canAttendMeetings(problem5Case2) << "\n";
    // Expected: true

    // Test Case 3: Back-to-Back Meetings
    vector<vector<int>> problem5Case3 = {
        {1, 3}, {3, 5}, {5, 8}
    };
    cout << "Test Case 3: "
         << canAttendMeetings(problem5Case3) << "\n";
    // Expected: true

    // Test Case 4: Unsorted Input
    vector<vector<int>> problem5Case4 = {
        {15, 20}, {0, 10}, {5, 15}
    };
    cout << "Test Case 4: "
         << canAttendMeetings(problem5Case4) << "\n";
    // Expected: false

    // Edge Case: Empty Input
    vector<vector<int>> problem5Case5;
    cout << "Edge Case: "
         << canAttendMeetings(problem5Case5) << "\n";
    // Expected: true
    cout << noboolalpha;

    // ========================================================
    // Problem 6: K Closest Points
    // ========================================================

    cout << "\n===== Problem 6: K Closest Points =====\n";

    // Test Case 1: Basic Example
    vector<vector<int>> problem6Points1 = {
        {1, 3}, {-2, 2}
    };
    int problem6K1 = 1;

    cout << "Test Case 1 - Sorting: ";
    printIntMatrix(kClosestUsingSorting(problem6Points1, problem6K1));

    vector<vector<int>> problem6Points1Heap = {
        {1, 3}, {-2, 2}
    };
    cout << "Test Case 1 - Heap: ";
    printIntMatrix(kClosestUsingHeap(problem6Points1Heap, problem6K1));
    // Expected: [[-2, 2]]

    // Test Case 2: Multiple Points
    vector<vector<int>> problem6Points2 = {
        {3, 3}, {5, -1}, {-2, 4}, {0, 1}
    };
    int problem6K2 = 2;

    cout << "Test Case 2 - Sorting: ";
    printIntMatrix(kClosestUsingSorting(problem6Points2, problem6K2));

    vector<vector<int>> problem6Points2Heap = {
        {3, 3}, {5, -1}, {-2, 4}, {0, 1}
    };
    cout << "Test Case 2 - Heap: ";
    printIntMatrix(kClosestUsingHeap(problem6Points2Heap, problem6K2));
    // Expected: [[0, 1], [3, 3]]

    // Test Case 3: Points on Different Axes
    vector<vector<int>> problem6Points3 = {
        {1, 0}, {0, 2}, {-1, 1}, {3, 0}
    };
    int problem6K3 = 2;

    cout << "Test Case 3 - Sorting: ";
    printIntMatrix(kClosestUsingSorting(problem6Points3, problem6K3));

    vector<vector<int>> problem6Points3Heap = {
        {1, 0}, {0, 2}, {-1, 1}, {3, 0}
    };
    cout << "Test Case 3 - Heap: ";
    printIntMatrix(kClosestUsingHeap(problem6Points3Heap, problem6K3));
    // Expected: [[1, 0], [-1, 1]]

    // Edge Case: k equals the number of points
    vector<vector<int>> problem6Points4 = {
        {1, 2}, {3, 4}, {0, 1}
    };
    int problem6K4 = 3;

    cout << "Edge Case - Sorting: ";
    printIntMatrix(kClosestUsingSorting(problem6Points4, problem6K4));

    vector<vector<int>> problem6Points4Heap = {
        {1, 2}, {3, 4}, {0, 1}
    };
    cout << "Edge Case - Heap: ";
    printIntMatrix(kClosestUsingHeap(problem6Points4Heap, problem6K4));
    // Expected: all three input points

    // ========================================================
    // Problem 7: K Most Frequent Values
    // ========================================================

    cout << "\n===== Problem 7: K Most Frequent Values =====\n";

    // Test Case 1: Different Frequencies
    vector<int> problem7Case1 = {
        1, 1, 1, 2, 2, 3
    };
    cout << "Test Case 1: ";
    printIntVector(topKFrequent(problem7Case1, 2));
    // Expected: [1, 2]

    // Test Case 2: Frequency Tie
    vector<int> problem7Case2 = {
        4, 4, 2, 2, 7, 7, 1
    };
    cout << "Test Case 2: ";
    printIntVector(topKFrequent(problem7Case2, 3));
    // Expected: [2, 4, 7]

    // Test Case 3: Negative Values
    vector<int> problem7Case3 = {
        -1, -1, 2, 2, 2, 3, 3
    };
    cout << "Test Case 3: ";
    printIntVector(topKFrequent(problem7Case3, 2));
    // Expected: [2, -1]

    // Edge Case: One Distinct Value
    vector<int> problem7Case4 = {
        5, 5, 5, 5
    };
    cout << "Edge Case: ";
    printIntVector(topKFrequent(problem7Case4, 1));
    // Expected: [5]

    // ========================================================
    // Problem 8: Merge K Sorted Arrays
    // ========================================================

    cout << "\n===== Problem 8: Merge K Sorted Arrays =====\n";

    // Test Case 1: Three Sorted Arrays
    vector<vector<int>> problem8Case1 = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };
    cout << "\nTest Case 1\n";
    cout << "Input: ";
    printIntMatrix(problem8Case1);
    cout << "Output: ";
    printIntVector(mergeKArrays(problem8Case1));
    // Expected: [1, 2, 3, 4, 5, 6, 7, 8, 9]

    // Test Case 2: Arrays of Different Sizes
    vector<vector<int>> problem8Case2 = {
        {1, 5},
        {2, 3, 8, 10},
        {4, 6, 7}
    };
    cout << "\nTest Case 2\n";
    cout << "Input: ";
    printIntMatrix(problem8Case2);
    cout << "Output: ";
    printIntVector(mergeKArrays(problem8Case2));
    // Expected: [1, 2, 3, 4, 5, 6, 7, 8, 10]

    // Test Case 3: Duplicate Values
    vector<vector<int>> problem8Case3 = {
        {1, 3, 5},
        {1, 3, 4},
        {2, 3, 6}
    };
    cout << "\nTest Case 3\n";
    cout << "Input: ";
    printIntMatrix(problem8Case3);
    cout << "Output: ";
    printIntVector(mergeKArrays(problem8Case3));
    // Expected: [1, 1, 2, 3, 3, 3, 4, 5, 6]

    // Test Case 4: Empty Array
    vector<vector<int>> problem8Case4 = {
        {},
        {1, 2, 3},
        {4, 5}
    };
    cout << "\nTest Case 4\n";
    cout << "Input: ";
    printIntMatrix(problem8Case4);
    cout << "Output: ";
    printIntVector(mergeKArrays(problem8Case4));
    // Expected: [1, 2, 3, 4, 5]

    // Edge Case: All Arrays Empty
    vector<vector<int>> problem8Case5 = {
        {},
        {},
        {}
    };
    cout << "\nEdge Case\n";
    cout << "Input: ";
    printIntMatrix(problem8Case5);
    cout << "Output: ";
    printIntVector(mergeKArrays(problem8Case5));
    // Expected: []

    // ========================================================
    // Problem 9: Kth Largest Element
    // ========================================================

    cout << "\n===== Problem 9: Kth Largest Element =====\n";

    // Test Case 1: Basic Example
    vector<int> problem9Case1 = {
        3, 2, 1, 5, 6, 4
    };
    cout << "\nTest Case 1\n";
    cout << "Input: ";
    printIntVector(problem9Case1);
    cout << "k: 2\n";
    cout << "Output: "
         << findKthLargest(problem9Case1, 2) << "\n";
    // Expected: 5

    // Test Case 2: Duplicate Values
    vector<int> problem9Case2 = {
        3, 2, 3, 1, 2, 4, 5, 5, 6
    };
    cout << "\nTest Case 2\n";
    cout << "Input: ";
    printIntVector(problem9Case2);
    cout << "k: 4\n";
    cout << "Output: "
         << findKthLargest(problem9Case2, 4) << "\n";
    // Expected: 4

    // Test Case 3: k = 1
    vector<int> problem9Case3 = {
        7, 2, 9, 4, 1
    };
    cout << "\nTest Case 3\n";
    cout << "Input: ";
    printIntVector(problem9Case3);
    cout << "k: 1\n";
    cout << "Output: "
         << findKthLargest(problem9Case3, 1) << "\n";
    // Expected: 9

    // Test Case 4: k = n
    vector<int> problem9Case4 = {
        4, 1, 7, 3
    };
    cout << "\nTest Case 4\n";
    cout << "Input: ";
    printIntVector(problem9Case4);
    cout << "k: 4\n";
    cout << "Output: "
         << findKthLargest(problem9Case4, 4) << "\n";
    // Expected: 1

    // Edge Case: All Values Are Equal
    vector<int> problem9Case5 = {
        5, 5, 5, 5, 5
    };
    cout << "\nEdge Case\n";
    cout << "Input: ";
    printIntVector(problem9Case5);
    cout << "k: 3\n";
    cout << "Output: "
         << findKthLargest(problem9Case5, 3) << "\n";
    // Expected: 5

    // ========================================================
    // Problem 10: Count Inversions
    // ========================================================

    cout << "\n===== Problem 10: Count Inversions =====\n";

    // Test Case 1: Basic Example
    vector<int> problem10Case1 = {
        2, 4, 1, 3, 5
    };
    cout << "\nTest Case 1\n";
    cout << "Input: ";
    printIntVector(problem10Case1);
    cout << "Output: "
         << countInversions(problem10Case1) << "\n";
    // Expected: 3

    // Test Case 2: Already Sorted
    vector<int> problem10Case2 = {
        1, 2, 3, 4, 5
    };
    cout << "\nTest Case 2\n";
    cout << "Input: ";
    printIntVector(problem10Case2);
    cout << "Output: "
         << countInversions(problem10Case2) << "\n";
    // Expected: 0

    // Test Case 3: Reverse Sorted
    vector<int> problem10Case3 = {
        5, 4, 3, 2, 1
    };
    cout << "\nTest Case 3\n";
    cout << "Input: ";
    printIntVector(problem10Case3);
    cout << "Output: "
         << countInversions(problem10Case3) << "\n";
    // Expected: 10

    // Test Case 4: Duplicate Values
    vector<int> problem10Case4 = {
        1, 3, 2, 3, 1
    };
    cout << "\nTest Case 4\n";
    cout << "Input: ";
    printIntVector(problem10Case4);
    cout << "Output: "
         << countInversions(problem10Case4) << "\n";
    // Correct expected value: 4

    // Edge Case: Empty Array
    vector<int> problem10Case5;
    cout << "\nEdge Case\n";
    cout << "Input: ";
    printIntVector(problem10Case5);
    cout << "Output: "
         << countInversions(problem10Case5) << "\n";
    // Expected: 0

    return 0;
}
