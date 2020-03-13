import React, { useState, useEffect } from 'react'
import Toolbar from './components/Toolbar'
import Visualization from './components/Visualization'
import Textbox from './components/Textbox'

import texts from './texts'
import { computeRandom, computeUniform } from './cppClient'

function App() {
  const [setSize, setSetSize] = useState(4)
  const [round, setRound] = useState(0)
  const [data, setData] = useState([{ nodes: [] }]) 
  const currRoundData = data[round]

  const loadRandomData = (setSize) => {
    InitRuntime().then((Module) => {
      console.log('random')
      const d = computeRandom(Module, setSize)
      setData(d)
      setRound(0)
    })
  }
  const loadUniformData = (setSize) => {
    InitRuntime().then((Module) => {
      console.log('uniform')
      const d = computeUniform(Module, setSize)
      setData(d)
      setRound(0)
    })
  }

  useEffect(() => {
    loadRandomData(setSize)
  }, [])
  
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
          loadRandomData={loadRandomData}
          loadUniformData={loadUniformData} />
      </div>
    </div>
  )
}

export default App
