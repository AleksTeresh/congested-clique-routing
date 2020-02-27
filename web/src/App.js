import React, { useState, useEffect } from 'react'
import Matrix from './components/Matrix'
import Toolbar from './components/Toolbar'

import { computeRouting } from './cppClient'

function groupPerSubclass(currRoundData, setSize) {
  const initGroupedNodes = [...(new Array(setSize))].map((p, i) => ({ id: i, messages: [] }))
  return {
    nodes: currRoundData.nodes.reduce((acc, node, idx) => {
        const subsetId = Math.floor(idx / setSize) 
        node.messages.forEach(message => {
          const srcSet = Math.floor(message.src / setSize)
          const finalDestSet = Math.floor(message.finalDest / setSize)
          acc[subsetId].messages.push({ src: srcSet, finalDest: finalDestSet })
        })
        return acc
    }, initGroupedNodes)
  }
}

function App() {
  const [tentativeSetSize, setTentativeSetSize] = useState(4)
  const [setSize, setSetSize] = useState(4)
  const [round, setRound] = useState(0)
  const [data, setData] = useState([{ nodes: [] }]) 
  const [groupedData, setGroupedData] = useState({ nodes: [] })
  const currRoundData = data[round]

  const loadData = (setSize) => {
    InitRuntime().then((Module) => {
      const d = computeRouting(Module, setSize)
      setData(d)
      setRound(0)
    })
  }

  useEffect(() => {
    loadData(setSize)
    // setGroupedData(groupPerSubclass(currRoundData, setSize))
  }, [setSize])
  useEffect(() => {
    setGroupedData(groupPerSubclass(currRoundData, setSize))
  }, [currRoundData])

  const handlePrev = () => setRound(round - 1)
  const handleNext = () => setRound(round + 1)

  return (
    <div className="App">
      <Matrix graphHistoryPoint={currRoundData} colorUpperLimit={setSize * setSize} />
      <Matrix graphHistoryPoint={groupedData} colorUpperLimit={setSize * setSize * setSize}/>
      <Toolbar
        round={round}
        handlePrev={handlePrev}
        handleNext={handleNext}
        tentativeSetSize={tentativeSetSize}
        setSize={setSize}
        setTentativeSetSize={setTentativeSetSize}
        setSetSize={setSetSize}
        loadData={loadData}
      />
    </div>
  );
}

export default App
