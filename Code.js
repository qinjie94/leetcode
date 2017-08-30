/**Determine whether an integer is a palindrome.
 * @param {number} x
 * @return {boolean}
 */
var isPalindrome = function(x) {
    str = x.toString();
    var i = 0;
    var j = str.length - 1;
    while(i < j) {
        if (str[i] !== str[j])
            return false;
        i++;
        j--
    }
    return true;
};
/**change Roman into Integer
 * @param {string} s
 * @return {number}
 */
var romanToInt = function(s) {
      var dict ={"I":1,"V":5,"X":10,"L":50,"C":100,"D":500,"M":1000}
      var result = 0;
      var curr,prev;
      for(var i = 0; i < s.length; i++){
          curr = dict[s[i]];
          result+=curr;
          if(i>0){
              prev = dict[s[i-1]];
              if(prev<curr ){
                  result= result-2*prev;
              }
          }
      }
      return result;
};
/**Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.
 * @param {string} s
 * @return {boolean}
 */
var isValid = function(s) {
    let stack = []
    let match = {"{":"}", "[":"]", "(":")"}
    for (var i = 0; i < s.length; i++) {
        if (s[i] === '{' || s[i] === '[' || s[i] === '(') stack.push(s[i])
        else if (s[i] === '}' || s[i] === ']' || s[i] === ')') {
            if (match[stack.pop()] !== s[i]) return false
        }
    }
    return stack.length === 0
};
/**Find the contiguous subarray within an array (containing at least one number) which has the largest sum. 
For example, given the array [-2,1,-3,4,-1,2,1,-5,4],
the contiguous subarray [4,-1,2,1] has the largest sum = 6.
求数组的连续子数组的最大和
 * @param {number[]} nums
 * @return {number}
 */
var maxSubArray = function(nums){
    var temp = 0;
    var result = nums[0];
    for(var i=0; i<nums.length; i++){
        temp += nums[i];
        //console.log(temp+" "+i);
        if(temp > result){
            result = temp;
            //console.log("result"+" "+result);
        }
        if(temp < 0){
            temp = 0;
        }
    }
    return result;
}
//优化解法
var maxSubArray1 = function(nums) {
    var max = nums[0], sum=0;
    for (i = 0; i<nums.length;i++) {
        sum = sum+nums[i];
        max = Math.max(max, sum);
        sum = Math.max(sum, 0);
    }
    return max;
};
/* * @param string s
 * @return the length of last word
 */
var lengthOfLastWord = function(s) {
    var temp = s.trim();
    var regExp = /\s+/g;
    var str = temp.replace(regExp, " ");
    var data = str.split(" ");
    return data[data.length-1].length
};
/**简化路径
 * @param {string} path
 * @return {string}
 */
 var simplifyPath = function(path) {
  var reg = /\/+/g;
  var path = path.replace(reg, '/');
    var temp = path.split("/");
    console.log(temp);
    var array = new Array();
    for(var i=0; i<temp.length; i++){
      if(temp[i]==="." || temp[i]==""){
        continue;
      }else if(temp[i]===".."){
        array.pop();
      }else{
        array.push(temp[i]);
      }
    }
    var result = "/"+array.join("/");
    return result;
};
/**删除链表中的值重复的节点
 * Definition for singly-linked list.
 * function ListNode(val) {
 *     this.val = val;
 *     this.next = null;
 * }
 */
/**
 * @param {ListNode} head
 * @return {ListNode}
 */
var deleteDuplicates = function(head) {
    let node = head;
    if(head === null) return head;
    while(node.next !== null) {
       if(node.val === node.next.val) {
           node.next = node.next.next;
       } else {
           node = node.next;
       }
    }
    return head;
};
/**Merge Sorted Array有序数组合并成一个数组
 * @param {number[]} nums1
 * @param {number} m
 * @param {number[]} nums2
 * @param {number} n
 * @return {void} Do not return anything, modify nums1 in-place instead.
 */
var merge = function(nums1, m, nums2, n) {
    while(m > 0 && n > 0){
        if(nums1[m - 1] > nums2[n - 1]){
            nums1[m+n-1] = nums1[m-1];
            m--;
        } else {
            nums1[m+n-1] = nums2[n-1];
            n--;
        }
    }
    while(n > 0) {
        nums1[n - 1] = nums2[n - 1];
        n--;
    }
};
/**二进制加减
 * @param {string} a
 * @param {string} b
 * @return {string}
 */
var addBinary = function(a, b) {
    var i = a.length - 1;
    var j = b.length - 1;
    var carry = 0;//进位
    var cnt = [];
    while(i >= 0 || j >= 0) {
        var sum = ((i < 0) ? 0 : (a.charAt(i--) - '0')) + ((j < 0) ? 0 : (b.charAt(j--) - '0')) + carry;
        cnt.push(sum % 2);
        carry = Math.floor(sum / 2);
    }
    if(carry !== 0) cnt.push(carry);
    return cnt.reverse().join('');//翻转，字符串连接
};
/**Given an array of size n, 
find the majority element. 
The majority element is the element 
that appears more than ⌊ n/2 ⌋ times.
 * @param {number[]} nums
 * @return {number}
 */
 var majorityElement = function(nums) {
    /*var i=0, count=0, res=0;
    //nums = [3,3,2,2,2];
    for(i=0; i < nums.length; i++){
         if(count === 0){
             res = nums[i];
             count++
         }
         else if(res === nums[i]){
             count++;
         }
         else{
             count--;
         }
    }
    
    return res;*/
    var temp = nums.sort((a,b)=>(a-b));
    return temp[Math.trunc(temp.length/2)]
};
