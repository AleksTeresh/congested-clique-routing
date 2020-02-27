import React, { useState, useEffect } from 'react'
import Matrix from './components/Matrix'
import Toolbar from './components/Toolbar'

import { computeRouting } from './cppClient'

function App() {
  const [tentativeSetSize, setTentativeSetSize] = useState(4)
  const [setSize, setSetSize] = useState(4)
  const [round, setRound] = useState(0)
  const [data, setData] = useState([{ nodes: [] }]) 

  const loadData = (setSize) => {
    InitRuntime().then((Module) => {
      const d = computeRouting(Module, setSize)
      setData(d)
      setRound(0)
    })
  }

  useEffect(() => {
    loadData(setSize)
  }, [])

  const handlePrev = () => setRound(round - 1)
  const handleNext = () => setRound(round + 1)

  return (
    <div className="App">
      <Matrix graphHistoryPoint={data[round]} />
      <Toolbar
        round={round}
        handlePrev={handlePrev}
        handleNext={handleNext}
        tentativeSetSize={tentativeSetSize}
        setTentativeSetSize={setTentativeSetSize}
        setSetSize={setSetSize}
        loadData={loadData}
      />
    </div>
  );
}

export default App
