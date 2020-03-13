function getRandomInt(max) {
  return Math.floor(Math.random() * Math.floor(max))
}

function fillInRandomMessages(messageMatrix) {
  const n = messageMatrix.length
  const sent = messageMatrix.map(p => p.reduce((a, b) => a + b, 0))
  const received = messageMatrix.map((_, i) => messageMatrix.reduce((a, b) => a + b[i], 0))

  for (let i = 0; i < messageMatrix.length; i++) {
    while (sent[i] < n) {
      const r = getRandomInt(n)
      if (received[r] < n) {
        received[r]++
        sent[i]++
        messageMatrix[i][r]++
      }
    }
  }

  return messageMatrix
}

export function computeCustom(Module, messageMatrix) {
  const data = fillInRandomMessages(messageMatrix)

  const vecOfVecs = new Module['Vec2']()
  for (let i = 0; i < data.length; i++) {
    const vec = new Module['Vec1']()

    for (let j = 0; j < data.length; j++) {
      for (let e = 0; e < data[i][j]; e++) {
        vec.push_back(j)
      }
    }

    vecOfVecs.push_back(vec)
  }

  const res = Module.custom_test(vecOfVecs)
  return cppToJSData(res)
}

export function computeRandom(Module, setSize) {
  const res = Module.random_test(setSize)
  return cppToJSData(res)
}

export function computeUniform(Module, setSize) {
  const res = Module.uniform_subset_test(setSize)
  return cppToJSData(res)
}

export function cppToJSData(res) {
  const data = []

  for (let i = 0; i < res.size(); i++) {
    const dataPoint = res.get(i)
    const nodes = []
    const n = dataPoint.nodes.size()

    for (let j = 0; j < n; j++) {
      const node = dataPoint.nodes.get(j)
      const messages = []
      const metaMessages = []

      for (let e = 0; e < node.messages.size(); e++) {
          const m = node.messages.get(e)  
          messages.push({
          src: m.src,
          finalDest: m.final_dest
          })   
      }
      for (let e = 0; e < node.message_counts.size(); e++) {
        const m = node.message_counts.get(e)  
        metaMessages.push({
            src: m.msg_src,
        })   
      }

      nodes.push({
          id: node.id,
          messages: messages,
          metaMessages: metaMessages
      })
    }
    
    data.push({nodes})
  }
  return data
}
