import React, { useState, useEffect } from 'react'
import Toolbar from './components/Toolbar'
import Visualization from './components/Visualization'
import Textbox from './components/Textbox'

import texts from './texts'
import { computeRandom, computeUniform, computeCustom } from './cppClient'

function App() {
  const [setSize, setSetSize] = useState(4)
  const [round, setRound] = useState(0)
  const [data, setData] = useState([{ nodes: [] }])
  const [editMode, setEditMode] = useState(false)
  const [customInput, setCustomInput] = useState([[]])
  const currRoundData = data[round]

  const loadRandomData = (setSize) => {
    InitRuntime().then((Module) => {
      const d = computeRandom(Module, setSize)
      setData(d)
      setRound(0)
    })
  }
  const loadUniformData = (setSize) => {
    InitRuntime().then((Module) => {
      const d = computeUniform(Module, setSize)
      setData(d)
      setRound(0)
    })
  }
  const toggleEditMode = (setSize) => {
    setEditMode(!editMode)
    setCustomInput([...Array(setSize * setSize)].map(p => ([...Array(setSize * setSize)].map(s => 0))))
  }
  
  const addToCustomInput = (row, col) => {
    const newCustomInput = [...customInput]
    const senginTotal = newCustomInput[row].reduce((a,b) => a + b, 0)
    const receivingTotal = newCustomInput.reduce((a,b) => a + b[col], 0)
    if (senginTotal < setSize * setSize && receivingTotal < setSize * setSize) {
      newCustomInput[row][col]++
      setCustomInput(newCustomInput) 
    }
  }
  const removeFromCustomInput = (row, col) => {
    const newCustomInput = [...customInput]
    newCustomInput[row][col] = Math.max(0, newCustomInput[row][col] - 1)
    setCustomInput(newCustomInput)
  }

  const startCustomCompute = () => {
    InitRuntime().then((Module) => {
      const d = computeCustom(Module, customInput)
      setData(d)
      setEditMode(false)
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
    minWidth: '800px',
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
          customInput={customInput}
          round={round}
          editMode={editMode}
          addToCustomInput={addToCustomInput}
          removeFromCustomInput={removeFromCustomInput} />
        <Toolbar
          round={round}
          handlePrev={handlePrev}
          handleNext={handleNext}
          editMode={editMode}
          setSize={setSize}
          setSetSize={setSetSize}
          loadRandomData={loadRandomData}
          loadUniformData={loadUniformData}
          toggleEditMode={toggleEditMode}
          startCustomCompute={startCustomCompute} />
      </div>
    </div>
  )
}

export default App
