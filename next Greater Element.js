var nextGreaterElement = function(findNums, nums) {
    var result = [];
    var temp;
    var flag;
    for(let i=0; i<findNums.length; i++){
    	flag = false;
        temp = nums.indexOf(findNums[i]);
        if(nums[temp+1]){
        	for(let j=temp+1; j<nums.length; j++){
        		if(nums[j]>findNums[i]){
        			result.push(nums[j]);
        			flag = true;
        			break;
        		}
       		}
       		if(!flag){
       			console.log(123);
       			result.push(-1);
       		}
	    }else{
	    	console.log(123);
	        result.push(-1);
	    }
    }
    return result;
};
var findNums = [2,1,3];
var nums = [2,3,1];
console.log(nextGreaterElement(findNums, nums));