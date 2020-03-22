import React from 'react'

import MarkdownRender from './MarkdownRender'

export default function Textbox({
  texts,
  round,
  editMode
}) {
  const wrapperStyle = {
    height: 'fit-content',
    padding: '0px 10px',
    margin: '10px',
    marginTop: '0px',
    position: 'sticky',
    top: '30px',
    minWidth: '580px'
  }
  return (
    <div
      style={wrapperStyle}>
      <MarkdownRender
        source={texts.intro} />
      <hr style={{ width: '100%', height: '0px' }} />
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
