/**
 * @param {string[]} words
 * @return {string[]}
 */
var findWords = function(words) {
    let result = [];
    let temp = 0;
    for(let i=0; i<words.length; i++){
        if(checkWord(words[i])){
           result.push(words[i]);
       }
    }
    return result;
};
function checkWord(word){
    let aph =[ "QqWwEeRrTtYyUuIiOoPp","AaSsDdFfGgHhJjKkLl","ZzXxCcVvBbNnMm"];
    var row;
    if(aph[0].indexOf(word[0])>-1){
            row = 0;
    }else if(aph[1].indexOf(word[0])>-1){
            row = 1;
    }else if(aph[2].indexOf(word[0])>-1){
            row = 2;
    }
    console.log(row);
    for(var i=0; i<word.length; i++){
        if(aph[row].indexOf(word[i])===-1){
            return false;
        }
    }
    return true;
}
/*方法二
var findWords = function(words) {
    let result = [];
    let temp = 0;
    for(let i=0; i<words.length; i++){
        if(checkWord(words[i])){
           result.push(words[i]);
       }
    }
    return result;
};
function checkWord(word){
    let row1 = "QqWwEeRrTtYyUuIiOoPp";
    let row2 = "AaSsDdFfGgHhJjKkLl";
    let row3 = "ZzXxCcVvBbNnMm";
    var temp = 0;
    state = 0;
    if(row1.indexOf(word[0])>-1){
            state = 1;
    }else if(row2.indexOf(word[0])>-1){
            state = 2;
    }else if(row3.indexOf(word[0])>-1){
            state = 3;
    }
    for(var i=0; i<word.length; i++){
        if(row1.indexOf(word[i])>-1){
            temp = 1;
        }else if(row2.indexOf(word[i])>-1){
            temp = 2;
        }else if(row3.indexOf(word[i])>-1){
            temp = 3;
        }
        //console.log(state);
        if(temp!==state){
            return false;
        }
    }
    return true;
}*/


var input =  ["Hello", "Alaska", "Dad", "Peace"];
var result = findWords(input);
console.log(result);