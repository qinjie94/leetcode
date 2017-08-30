/*数组中只有一个数只出现一次，其他都出现两次。求出现一次的数
*/
var singleNumber = function(nums) {
    // var temp = new Set(nums);
    // console.log(temp[0]);
    // var sum = 0;
    // for(let key of temp){
    //     sum += key;
    // }
    // sum *= 2;
    // //console.log(sum);
    // for(let i=0; i<nums.length; i++){
    //     sum -= nums[i];
    // }
    // //console.log(sum);
    // return sum;
    let result = 0;
    for(let i=0; i<nums.length; i++){
        result ^=nums[i];
    }
    return result;
};
console.log(singleNumber([1]));