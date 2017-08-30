/*
 * Island Perimeter 
 *计算周长
*/
var islandPerimeter = function(grid) {
    var result = 0;
    var width = grid[0].length;
    var height = grid.length;
    for(let i=0; i<height; i++){
    	for(let j=0; j<width; j++){
    		if(grid[i][j]===0){
    			continue;
    		}else{
    			result += countChair(grid, i, j);
    		}
    	}
    }
    return result;
};
function countChair(g, i, j){
	var start = 4;
	if(g[i][j+1]&&g[i][j+1]===1){
		start--;
	}
	if(g[i][j-1]&&g[i][j-1]===1){
		start--;
	}
	if(g[i+1]){
		if(g[i+1][j]&&g[i+1][j]===1){
		start--;
		}
	}
	if(g[i-1]){
		if(g[i-1][j]&&g[i-1][j]===1){
		start--;
		}
	}
	return start;
}
var grid = [[0,0,1]];
console.log(islandPerimeter(grid));