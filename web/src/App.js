import React, { useState, useEffect } from 'react'
import Toolbar from './components/Toolbar'
import Visualization from './components/Visualization'
import Textbox from './components/Textbox'

import texts from './texts'
import { computeRouting } from './cppClient'

function App() {
  const [setSize, setSetSize] = useState(4)
  const [round, setRound] = useState(0)
  const [data, setData] = useState([{ nodes: [] }]) 
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
  }, [setSize])
  
  const handlePrev = () => setRound(round - 1)
  const handleNext = () => setRound(round + 1)

  const visualizationStyle = {
    marginLeft: 'auto',
    marginRight: '20px',
    maxWidth: '1200px',
    width: 'fit-content'
  }
  const wrappperStyle = { display: 'flex' }

  return (
    <div className="App" style={wrappperStyle}>
      <Textbox
        texts={texts}
        round={round} />
      <div style={visualizationStyle}>
        <Visualization
          data={data}
          round={round} />
        <Toolbar
          round={round}
          handlePrev={handlePrev}
          handleNext={handleNext}
          setSize={setSize}
          setSetSize={setSetSize}
          loadData={loadData} />
      </div>
    </div>
  )
}

export default App
