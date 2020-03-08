export function computeRouting(Module, setSize) {
  const res = Module.random_test(setSize)
  const n = setSize * setSize
  const data = []
  for (let i = 0; i < res.size(); i++) {
      const dataPoint = res.get(i)
      const nodes = []
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
