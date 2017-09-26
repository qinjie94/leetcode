
/**
 * @param {number[]} nums
 * @return {string[]}
 */
var findRelativeRanks = function(nums) {
    var result = [];
    var map = new Map();
    var temp;
    for(let i=0; i<nums.length; i++){
        map.set(nums[i], i);
    }
    var sortArray = sort(nums);
    console.log(sortArray);
    for(let i=0; i<sortArray.length; i++){
        temp = i+1;
        result[map.get(sortArray[i])] = temp.toString();
    }
    console.log(result);
    [result[result.indexOf("1")],result[result.indexOf("2")],result[result.indexOf("3")]] = ["Gold Medal","Sliver Medal","Bronze Medal"]
    return result;
};

function sort(array){
    let count=0;
    function quikSort(left, right){
        count++;
        let key = array[left];
        let i = left;
        let j = right;
        if(i<j){
            while(i<j){
                while(i<j&&key>=array[j]){
                    j--;
                }
                array[i] = array[j];
                while(i<j&&key<=array[i]){
                    i++;
                }
                array[j] = array[i];
            }
            array[i] = key;
            //console.log(array);
            quikSort(left, i-1);
            quikSort(i+1, right);
        }else{
            return null;
        }
    }
    quikSort(0, array.length-1);
    //console.log(count);
    return array;
}
var nums = [1];
var result = findRelativeRanks(nums);
console.log("result: "+result);