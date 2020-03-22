import React, { useState } from 'react'

const MAX_NUM_OF_ROUNDS = 16

const buttonStyle = {
  margin: '20px',
  marginTop: '5px',
  fontSize: '15px',
  lineHeight: '2.5',
  padding: '0 20px',
  fontSize: '1rem',
  border: 0,
  borderRadius: '10px',
  backgroundColor: '#f5f4f2',
  boxShadow: 'inset 1px 1px 1px rgba(255, 255, 255, .6), inset -1px -1px 1px rgba(0, 0, 0, .6)'
}
const wrapperStyle = {
  margin: '80px',
  marginTop: '20px'
}
const nextPrevButtonStyle = {
  width: '500px'
}
const inputControlsWrapperStyle = {
  margin: '20px'
}

export default function Toolbar({
  round,
  handlePrev,
  handleNext,
  setSize,
  setSetSize,
  editMode,
  loadRandomData,
  loadUniformData,
  toggleEditMode,
  startCustomCompute
}) {
  const [tentativeSetSize, setTentativeSetSize] = useState(4)

  return (
    <div style={wrapperStyle}>
      <PrevNextButtons
        round={round}
        editMode={editMode}
        handlePrev={handlePrev}
        handleNext={handleNext} />
      <InputControls
        setSize={setSize}
        setSetSize={setSetSize}
        tentativeSetSize={tentativeSetSize}
        setTentativeSetSize={setTentativeSetSize}
        editMode={editMode}
        loadUniformData={loadUniformData}
        loadRandomData={loadRandomData}
        toggleEditMode={toggleEditMode}
        startCustomCompute={startCustomCompute} />
    </div>
  )
}

function InputControls({
  setSize,
  setSetSize,
  tentativeSetSize,
  setTentativeSetSize,
  loadRandomData,
  loadUniformData,
  toggleEditMode,
  editMode,
  startCustomCompute
}) {
  return <>
    <div style={inputControlsWrapperStyle}>
      <label id="numberOfNodes">{'Size of subset ' + tentativeSetSize}</label>
      <br />
      <input
        disabled={editMode}
        onInput={(ev) => setTentativeSetSize(Number(ev.target.value))}
        type="range"
        min="2"
        max="7"
        value={tentativeSetSize}
        className="slider"
        id="nodesRange" />
    </div>
    <div>
      <button
        disabled={editMode}
        style={buttonStyle}
        onClick={() => {
          loadRandomData(Number(tentativeSetSize))
          setSetSize(Number(tentativeSetSize))
        }}
        id="computeBtn">
        Compute random
      </button>
      <button
        disabled={editMode}
        style={buttonStyle}
        onClick={() => {
          loadUniformData(Number(tentativeSetSize))
          setSetSize(Number(tentativeSetSize))
        }}
        id="computeBtn">
        Compute uniform
      </button>
      <button
        style={buttonStyle}
        onClick={() => {
          setSetSize(Number(tentativeSetSize))
          toggleEditMode(Number(tentativeSetSize))
        }}
        id="computeBtn">
        {editMode ? 'Cancel' : 'Edit input'}
      </button>
      <button
        disabled={!editMode}
        style={buttonStyle}
        onClick={() => startCustomCompute()}
        id="computeBtn">
        Fill the rest and compute
      </button>
    </div>
  </>
}

function PrevNextButtons({
  round,
  handlePrev,
  handleNext,
  editMode
}) {
  const roundIndicatorStyle = {
    float: 'right',
    marginTop: '5px'
  }
  return (
    <div style={nextPrevButtonStyle}>
      <button
        disabled={editMode || round === 0}
        type="button"
        style={buttonStyle}
        onClick={handlePrev}
        id="prevBtn">
        Prev round
      </button>
      <button
        disabled={editMode || round === MAX_NUM_OF_ROUNDS}
        type="button"
        style={buttonStyle}
        onClick={handleNext}
        id="nextBtn">
        Next round
      </button>
      <div style={roundIndicatorStyle}>
        <label>{'Round No: ' + round}</label>
      </div>
    </div>
  )
}
