/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define ADD_MANY(vector,...) vector.insert(vector.end(),{__VA_ARGS__})

//==============================================================================
RotarySlider::RotarySlider(SineWaveSynthesizerAudioProcessor& audioProcessor, char* varName):nameLabel("nameLabel",varName) {
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

    setTextBoxStyle(juce::Slider::TextBoxBelow,
        true,
        getTextBoxWidth(),
        getTextBoxHeight());
    setTextValueSuffix("s");

    setRotaryParameters(-2.5 + 6.283, 2.5 + 6.283, true);
    setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, varName,*this));

    nameLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(nameLabel);
};

void RotarySlider::resized() {
    juce::Slider::resized();
    nameLabel.setBounds(getLocalBounds().getProportion(juce::Rectangle<float>(0,.8, 1, .2)));
}



SineWaveSynthesizerAudioProcessorEditor::SineWaveSynthesizerAudioProcessorEditor(SineWaveSynthesizerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),waveFormComboBox("wave form")
{ 
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    waveFormComboBox.addItemList({ "Sine","Square","Triangle","Saw" },1);
    waveFormComboBox.setSelectedId(1);
    waveFormComboBoxAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "waveform", waveFormComboBox));
    ADD_MANY(main, 
        new RotarySlider(audioProcessor, "level"),
    );
    ADD_MANY(adsrTime,
        new RotarySlider(audioProcessor, "aTime"),
        new RotarySlider(audioProcessor, "dTime"),
        new RotarySlider(audioProcessor, "sTime"),
        new RotarySlider(audioProcessor, "rTime"),
        );
    ADD_MANY(adsrVel,
        new RotarySlider(audioProcessor, "aVel"),
        new RotarySlider(audioProcessor, "dVel"),
        new RotarySlider(audioProcessor, "sVel"),
        new RotarySlider(audioProcessor, "rVel"),
        );
    
    addAndMakeVisible(waveFormComboBox);
    for (auto &it : main) {
        addAndMakeVisible(it);
    }
    for (auto& it : adsrTime) {
        addAndMakeVisible(it);
    }
    for (auto& it : adsrVel) {
        addAndMakeVisible(it);
    }
    setSize(400, 300);

}

SineWaveSynthesizerAudioProcessorEditor::~SineWaveSynthesizerAudioProcessorEditor()
{
    // Delete it, delete it, delete it.
    for (auto& it : main) {
        delete it;
    }
    for (auto& it : adsrTime) {
        delete it;
    }
    for (auto& it : adsrVel) {
        delete it;
    }
}

//==============================================================================
void SineWaveSynthesizerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SineWaveSynthesizerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    int sliderWidth = area.getWidth() / 2;
    int sliderHeight = area.getHeight() / 4;
    int x = 100;
    int y = 100;

    juce::FlexBox fbMain;
    fbMain.flexWrap = juce::FlexBox::Wrap::wrap;
    fbMain.justifyContent = juce::FlexBox::JustifyContent::center;
    fbMain.alignContent = juce::FlexBox::AlignContent::center;

    for (auto& it :  main) {
        fbMain.items.add(juce::FlexItem(*it).withMinWidth(70.0f).withMinHeight(90.0f));
    }
    fbMain.items.add(juce::FlexItem(waveFormComboBox).withMinWidth(150.0f).withMinHeight(70.0f));
    fbMain.performLayout(area.toFloat());

    juce::FlexBox fbAdsrTime;
    fbAdsrTime.flexWrap = juce::FlexBox::Wrap::wrap;
    fbAdsrTime.justifyContent = juce::FlexBox::JustifyContent::center;
    fbAdsrTime.alignContent = juce::FlexBox::AlignContent::center;

    for (auto& it : adsrTime) {
        fbAdsrTime.items.add(juce::FlexItem(*it).withMinWidth(70.0f).withMinHeight(90.0f));
    }
    fbAdsrTime.performLayout(area.toFloat());

    juce::FlexBox fbAdsrVel;
    fbAdsrVel.flexWrap = juce::FlexBox::Wrap::wrap;
    fbAdsrVel.justifyContent = juce::FlexBox::JustifyContent::center;
    fbAdsrVel.alignContent = juce::FlexBox::AlignContent::center;

    for (auto& it : adsrVel) {
        fbAdsrVel.items.add(juce::FlexItem(*it).withMinWidth(70.0f).withMinHeight(90.0f));
    }
    fbAdsrVel.performLayout(area.toFloat());



    juce::FlexBox fb;                                               
    fb.flexWrap = juce::FlexBox::Wrap::wrap;                        
    fb.justifyContent = juce::FlexBox::JustifyContent::center;      
    fb.alignContent = juce::FlexBox::AlignContent::center;          
    
    fb.items.addArray({ 
        juce::FlexItem((float)getWidth() , (float)getHeight()*0.333,fbMain),
        juce::FlexItem((float)getWidth() , (float)getHeight()*0.333,fbAdsrTime) ,
        juce::FlexItem((float)getWidth() , (float)getHeight()*0.333,fbAdsrVel) });
    fb.performLayout(area.toFloat());
}