import React from 'react'

import MarkdownRender from './MarkdownRender'

export default function Textbox({
  texts,
  round,
  editMode
}) {
  const wrapperStyle = {
    border: '1px solid black',
    height: 'fit-content',
    display: 'inline-grid',
    padding: '10px',
    margin: '10px',
    marginTop: '30px'
  }
  return (
    <div
      style={wrapperStyle}>
      <MarkdownRender
        source={
          editMode
          ? texts.editInstructions
          : texts
              .algorithmSteps[round]
              .title
        } />
      <MarkdownRender
        source={
          editMode
          ? ''
          : texts
              .algorithmSteps[round]
              .detail} />
    </div>
  )
}
