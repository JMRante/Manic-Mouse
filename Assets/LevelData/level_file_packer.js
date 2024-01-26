const fs = require('fs')

fs.readdir('./', (err, files) => {
  let levelsDataString = ''
  
  files
  .filter(file => {
    return file.endsWith('.json')
  })
  .forEach(file => {
    console.log(`Packing ${file}`)
    const fileDataRaw = fs.readFileSync(file)
    const fileData = JSON.parse(fileDataRaw)

    let levelData = {}

    let tilesetFileName = fileData.tilesets.find(x => x.source.startsWith('tileset')).source

    levelData.tilemap = {}

    switch (tilesetFileName) {
      case 'tileset1.tsx': 
        levelData.tilemap.id = 0 
        break
      case 'tileset2.tsx': 
        levelData.tilemap.id = 1 
        break
      case 'tileset3.tsx': 
        levelData.tilemap.id = 2 
        break
      case 'tileset4.tsx': 
        levelData.tilemap.id = 3 
        break
      case 'tileset5.tsx': 
        levelData.tilemap.id = 4 
        break
    }

    levelData.tilemap.data = fileData.layers
      .find(x => x.name == 'tilemap').data
      .map(x => {if (x == 7) {
        return 0
      } else {
        return 1
      }})
    
    let timeLimit = fileData.properties.find(x => x.name == 'time_limit').value;
    levelData.timeLimit = timeLimit;

    let objectList = fileData.layers.find(x => x.name == 'objects').objects

    levelData.movingBlocks = []

    objectList.forEach(obj => {
      switch (obj.gid) {
        case 65:
          levelData.start = {}
          levelData.start.x = obj.x
          levelData.start.y = obj.y
          break
        case 68:
          levelData.cheese = {}
          levelData.cheese.x = obj.x
          levelData.cheese.y = obj.y
          break
        case 89:
          levelData.redKey = {}
          levelData.redKey.x = obj.x
          levelData.redKey.y = obj.y
          break
        case 90:
          levelData.yellowKey = {}
          levelData.yellowKey.x = obj.x
          levelData.yellowKey.y = obj.y
          break
        case 91:
          levelData.blueKey = {}
          levelData.blueKey.x = obj.x
          levelData.blueKey.y = obj.y
          break
        case 81:
          levelData.redDoor = {}
          levelData.redDoor.x = obj.x
          levelData.redDoor.y = obj.y
          break
        case 82:
          levelData.yellowDoor = {}
          levelData.yellowDoor.x = obj.x
          levelData.yellowDoor.y = obj.y
          break
        case 83:
          levelData.blueDoor = {}
          levelData.blueDoor.x = obj.x
          levelData.blueDoor.y = obj.y
          break
        case 74:
        case 75:
          let movingBlock = {}
          movingBlock.behavior = obj.gid == 74 ? 0 : 1
          movingBlock.x = obj.x
          movingBlock.y = obj.y

          levelData.movingBlocks.push(movingBlock)
          break
      }
    })

    let levelDataString = ''

    levelDataString += levelData.timeLimit + '\n'
    levelDataString += levelData.start.x + '\n'
    levelDataString += levelData.start.y + '\n'
    levelDataString += levelData.cheese.x + '\n'
    levelDataString += levelData.cheese.y + '\n'
    levelDataString += (levelData.redKey ? 1 : 0) + '\n'
    levelDataString += (levelData.redKey ? levelData.redKey.x : 0) + '\n'
    levelDataString += (levelData.redKey ? levelData.redKey.y : 0) + '\n'
    levelDataString += (levelData.yellowKey ? 1 : 0) + '\n'
    levelDataString += (levelData.yellowKey ? levelData.yellowKey.x : 0) + '\n'
    levelDataString += (levelData.yellowKey ? levelData.yellowKey.y : 0) + '\n'
    levelDataString += (levelData.blueKey ? 1 : 0) + '\n'
    levelDataString += (levelData.blueKey ? levelData.blueKey.x : 0) + '\n'
    levelDataString += (levelData.blueKey ? levelData.blueKey.y : 0) + '\n'
    levelDataString += (levelData.redDoor ? 1 : 0) + '\n'
    levelDataString += (levelData.redDoor ? levelData.redDoor.x : 0) + '\n'
    levelDataString += (levelData.redDoor ? levelData.redDoor.y : 0) + '\n'
    levelDataString += (levelData.yellowDoor ? 1 : 0) + '\n'
    levelDataString += (levelData.yellowDoor ? levelData.yellowDoor.x : 0) + '\n'
    levelDataString += (levelData.yellowDoor ? levelData.yellowDoor.y : 0) + '\n'
    levelDataString += (levelData.blueDoor ? 1 : 0) + '\n'
    levelDataString += (levelData.blueDoor ? levelData.blueDoor.x : 0) + '\n'
    levelDataString += (levelData.blueDoor ? levelData.blueDoor.y : 0) + '\n'
    levelDataString += levelData.movingBlocks.length + '\n'
    levelData.movingBlocks.forEach(movingBlock => {
      levelDataString += movingBlock.behavior + '\n'
      levelDataString += movingBlock.x + '\n'
      levelDataString += movingBlock.y + '\n'
    })
    levelDataString += levelData.tilemap.id + '\n'
    levelDataString += levelData.tilemap.data.join('') + '\n'

    levelsDataString += levelDataString
  })

  try {
    fs.writeFileSync('./level_data.lvls', levelsDataString)
  } catch (err) {
    console.error(err)
  }
})